#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/flashlight"
#define SOCKET_PATH "/tmp/flashlightd.sock"
#define FLASHLIGHT_MAGIC 'S'
#define FLASHLIGHTIOC_X_SET_DRIVER _IOWR(FLASHLIGHT_MAGIC, 30, int)
#define FLASH_IOC_SET_TIME_OUT_TIME_MS _IOR(FLASHLIGHT_MAGIC, 100, int)
#define FLASH_IOC_SET_DUTY _IOR(FLASHLIGHT_MAGIC, 110, int)
#define FLASH_IOC_SET_ONOFF _IOR(FLASHLIGHT_MAGIC, 115, int)

/* Exact userspace ABI from CPH2185 flashlight.h. */
struct flashlight_user_arg {
    int type_id;
    int ct_id;
    int arg;
};

static int flashlight_ioctl(int fd, unsigned long request, int value)
{
    struct flashlight_user_arg a = {
        /*
         * User IDs are 1-based. The kernel maps (1, 1) to
         * internal type/CT indexes (0, 0).
         */
        .type_id = 1,
        .ct_id = 1,
        .arg = value
    };
    return ioctl(fd, request, &a);
}

static int send_command(const char *command)
{
    int fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_PATH);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect daemon (start it first: flashlightd daemon)");
        close(fd);
        return 1;
    }

    if (write(fd, command, strlen(command)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    char reply[128] = {0};
    ssize_t n = read(fd, reply, sizeof(reply) - 1);
    if (n > 0) printf("%s", reply);
    close(fd);
    return (n > 0 && strncmp(reply, "OK", 2) == 0) ? 0 : 1;
}

static int run_daemon(void)
{
    int dev = open(DEVICE_PATH, O_RDWR | O_CLOEXEC);
    if (dev < 0) { perror("open /dev/flashlight"); return 1; }

    if (flashlight_ioctl(dev, FLASHLIGHTIOC_X_SET_DRIVER, 1) < 0) {
        perror("FLASHLIGHTIOC_X_SET_DRIVER");
        close(dev);
        return 1;
    }

    int server = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (server < 0) {
        perror("socket");
        close(dev);
        return 1;
    }

    unlink(SOCKET_PATH);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_PATH);

    if (bind(server, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
        chmod(SOCKET_PATH, 0666) < 0 || listen(server, 4) < 0) {
        perror("bind/chmod/listen");
        close(server);
        unlink(SOCKET_PATH);
        close(dev);
        return 1;
    }

    fprintf(stderr, "flashlightd: ready; holding %s open\n", DEVICE_PATH);
    int running = 1;
    while (running) {
        int client = accept4(server, NULL, NULL, SOCK_CLOEXEC);
        if (client < 0) {
            if (errno == EINTR) continue;
            perror("accept");
            break;
        }

        char cmd[32] = {0};
        ssize_t n = read(client, cmd, sizeof(cmd) - 1);
        const char *reply = "OK\n";

        if (n <= 0) {
            reply = "ERR: empty command\n";
        } else if (strncmp(cmd, "on", 2) == 0) {
            if (flashlight_ioctl(dev, FLASH_IOC_SET_TIME_OUT_TIME_MS, 0) < 0 ||
                flashlight_ioctl(dev, FLASH_IOC_SET_DUTY, 1) < 0 ||
                flashlight_ioctl(dev, FLASH_IOC_SET_ONOFF, 1) < 0) {
                perror("flashlight ON ioctl");
                reply = "ERR: ON ioctl failed\n";
            }
        } else if (strncmp(cmd, "off", 3) == 0) {
            if (flashlight_ioctl(dev, FLASH_IOC_SET_ONOFF, 0) < 0) {
                perror("flashlight OFF ioctl");
                reply = "ERR: OFF ioctl failed\n";
            }
        } else if (strncmp(cmd, "stop", 4) == 0) {
            if (flashlight_ioctl(dev, FLASH_IOC_SET_ONOFF, 0) < 0)
                perror("flashlight OFF during stop");
            running = 0;
        } else {
            reply = "ERR: use on, off, or stop\n";
        }

        (void)write(client, reply, strlen(reply));
        close(client);
    }

    (void)flashlight_ioctl(dev, FLASH_IOC_SET_ONOFF, 0);
    close(server);
    unlink(SOCKET_PATH);
    close(dev);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s {daemon|on|off|stop}\n", argv[0]);
        return 2;
    }
    if (strcmp(argv[1], "daemon") == 0) return run_daemon();
    if (strcmp(argv[1], "on") == 0 ||
        strcmp(argv[1], "off") == 0 ||
        strcmp(argv[1], "stop") == 0)
        return send_command(argv[1]);
    fprintf(stderr, "Unknown command: %s\n", argv[1]);
    return 2;
}

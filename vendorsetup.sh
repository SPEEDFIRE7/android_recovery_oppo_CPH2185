#!/usr/bin/env bash
#
# CPH2185 TWRP 12.1 build environment
# Adapted from the CPH2185 OrangeFox fox_12.1 device setup.
#
FDEVICE="CPH2185"

twrp_get_target_device() {
    local chkdev
    chkdev=$(echo "$BASH_SOURCE" | grep -w "$FDEVICE")
    if [ -n "$chkdev" ]; then
        TARGET_DEVICE="$FDEVICE"
    else
        chkdev=$(set | grep BASH_ARGV | grep -w "$FDEVICE")
        [ -n "$chkdev" ] && TARGET_DEVICE="$FDEVICE"
    fi
}

if [ -z "$1" ] && [ -z "$TARGET_DEVICE" ]; then
    twrp_get_target_device
fi

if [ "$1" = "$FDEVICE" ] || [ "$TARGET_DEVICE" = "$FDEVICE" ]; then
    export TW_DEFAULT_LANGUAGE="en"
    export LC_ALL="C"
    export ALLOW_MISSING_DEPENDENCIES=true
fi

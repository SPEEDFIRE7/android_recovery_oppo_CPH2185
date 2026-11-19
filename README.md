# OrangeFox Recovery for OPPO A15 (CPH2185)

Device tree branch: `OFOX`  
Base device configuration: TWRP 12.1 tree for MediaTek MT6765  
OrangeFox build target: `recoveryimage`

## OrangeFox configuration

This branch keeps the original CPH2185 partition, kernel, DTBO, fstab and ramdisk configuration, and adds OrangeFox build settings in `vendorsetup.sh`:

- Vanilla OrangeFox build (non-MIUI device).
- Prebuilt kernel support.
- Default Trustonic Keymaster version 4.0, matching the bundled Keymaster 4.0 service.
- OrangeFox App Manager.

## Build

Run **Actions → OrangeFox Recovery Builder → Run workflow** on the `OFOX` branch.

The workflow uses OrangeFox's 12.1 sync script and clones this branch into
`device/oppo/CPH2185`. It builds `recoveryimage` and publishes the generated
OrangeFox ZIP as a GitHub Release.

Required repository secret:
- `GH_TOKEN`: token with permission to create releases.

## Important

This is an initial OrangeFox integration, not a verified recovery image.
The CPH2185-specific kernel, DTBO, partition layout and ramdisk have been retained
from the existing device tree. The resulting image must be built and tested on
the device before it can be considered bootable or safe to flash.

Original device: OPPO A15 / CPH2185 (MediaTek MT6765).

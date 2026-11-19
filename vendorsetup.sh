#!/system/bin/sh
# OrangeFox build configuration for OPPO A15 (CPH2185)
# This tree uses a prebuilt MT6765 kernel and Trustonic Keymaster 4.0.

export FOX_BUILD_DEVICE="CPH2185"
export FOX_VANILLA_BUILD=1
export OF_FORCE_PREBUILT_KERNEL=1
export OF_DEFAULT_KEYMASTER_VERSION=4.0
export FOX_ENABLE_APP_MANAGER=1

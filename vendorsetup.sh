#!/usr/bin/env bash

echo "Starting OrangeFox build for OPPO A15 (CPH2185)"

add_lunch_combo twrp_CPH2185-eng

# Build environment
export TW_DEFAULT_LANGUAGE="en"
export ALLOW_MISSING_DEPENDENCIES=true
export LC_ALL="C"

# OrangeFox maintainer information
export OF_MAINTAINER="SPEEDFIRE7"
export FOX_BUILD_TYPE="Unofficial"

# OrangeFox features
export OF_DISABLE_MIUI_SPECIFIC_FEATURES=1
export FOX_USE_NANO_EDITOR=1
export FOX_ENABLE_APP_MANAGER=1
export FOX_USE_BASH_SHELL=1
export FOX_ASH_IS_BASH=1
export FOX_USE_XZ_UTILS=1
export FOX_USE_ZSTD_BINARY=1
export OF_USE_LZ4_COMPRESSION=1
export FOX_NO_SAMSUNG_SPECIAL=1

# OPPO A15 uses the prebuilt kernel defined in BoardConfig.mk
export OF_FORCE_PREBUILT_KERNEL=1

# CPH2185 uses dynamic partitions
export OF_ENABLE_LPTOOLS=1

# Avoid reflashing the currently installed OrangeFox build
export OF_NO_REFLASH_CURRENT_ORANGEFOX=1

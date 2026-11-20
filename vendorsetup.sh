#!/usr/bin/env bash
#
# CPH2185 TWRP 12.1 build environment.
# Common environment settings adapted from:
# SPEEDFIRE7/android_device_ofox_CPH2185recovery (fox_12.1).
#
# This is a single-device TWRP tree, so apply the shared build settings
# directly. OrangeFox-only FOX_*/OF_* variables are intentionally omitted.

export TW_DEFAULT_LANGUAGE="en"
export LC_ALL="C"
export ALLOW_MISSING_DEPENDENCIES=true

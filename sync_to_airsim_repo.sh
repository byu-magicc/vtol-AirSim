#!/bin/bash

if [[ -z "${AIRSIMPATH}" ]]; then
    echo ""
    echo "AIRSIMPATH not set."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
fi

set -e
set -x

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

# Sync changes to AirLib source files
rsync -a --delete Source/AirLib/include "$AIRSIMPATH"/AirLib
rsync -a --delete Source/AirLib/src     "$AIRSIMPATH"/AirLib


# Sync changes to AirSim plugin
plugin_source="$AIRSIMPATH"/Unreal/Plugins/AirSim/Source

# These are listed explicitly to avoid copying Source/AirLib to AirSim/Unreal/Plugins/AirSim/Source
rsync -a --delete Source/*.h            "$plugin_source"
rsync -a --delete Source/*.cpp          "$plugin_source"
rsync -a --delete Source/Recording      "$plugin_source"
rsync -a --delete Source/SimHUD         "$plugin_source"
rsync -a --delete Source/SimJoyStick    "$plugin_source"
rsync -a --delete Source/SimMode        "$plugin_source"
rsync -a --delete Source/UnrealSensors  "$plugin_source"
rsync -a --delete Source/Vehicles       "$plugin_source"
rsync -a --delete Source/Weather        "$plugin_source"

# Sync changes to VTOL assets
old_dir="$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/Tiltrotor
new_dir="$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/VTOL
if [[ -d "$old_dir" ]]; then
    mkdir "$new_dir"
    mv "$old_dir" "$new_dir"
fi
rsync -a --delete Content/VTOL/Tiltrotor "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/VTOL

popd >/dev/null
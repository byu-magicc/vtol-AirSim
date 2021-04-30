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
rsync -a --delete Source/AirLib/include $AIRSIMPATH/AirLib
rsync -a --delete Source/AirLib/src     $AIRSIMPATH/AirLib


# Sync changes to AirSim plugin
# These are listed explicitly to avoid copying Source/AirLib to $plugin_source
plugin_source=$AIRSIMPATH/Unreal/Plugins/AirSim/Source

rsync -a --delete Source/*.h Source/*.cpp $plugin_source
rsync -a --delete Source/Recording        $plugin_source
rsync -a --delete Source/SimHUD           $plugin_source
rsync -a --delete Source/SimJoyStick      $plugin_source
rsync -a --delete Source/SimMode          $plugin_source
rsync -a --delete Source/UnrealSensors    $plugin_source
rsync -a --delete Source/Vehicles         $plugin_source
rsync -a --delete Source/Weather          $plugin_source

# Sync changes to Tiltrotor assets
rsync -a --delete Content/Tiltrotor $AIRSIMPATH/Unreal/Plugins/AirSim/Content

popd >/dev/null
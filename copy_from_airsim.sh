#!/bin/bash

if [[ -z "${AIRSIMPATH}" ]]; then
    echo ""
    echo "AIRSIMPATH not set."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
fi

set -e

# this will print stuff if there are any uncommitted changes in the working tree
changes="$(git status --porcelain=v1 2>/dev/null)"
if [[ -n "$changes" ]] && [[ $changes != " M copy_from_airsim.sh" ]]; then
    echo ""
    echo "Please commit or stash your changes in 'vtol-AirSim-Plugin' and 'Source/AirLib' before running this script!"
    echo ""
    echo "    That way you can easily see what changed if anything goes wrong."
    echo ""
    echo ""
    exit 1
else
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    pushd "$SCRIPT_DIR" >/dev/null

    set -x
    # Sync changes to AirLib source files
    rsync -a --delete "$AIRSIMPATH"/AirLib/include  Source/AirLib
    rsync -a --delete "$AIRSIMPATH"/AirLib/src      Source/AirLib

    # Sync changes to AirSim plugin source files
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/*.h            ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/*.cpp          ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/Recording      ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/SimHUD         ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/SimJoyStick    ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/SimMode        ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/UnrealSensors  ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/Vehicles       ./Source
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Source/Weather        ./Source

    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/AirSim.uplugin  ./

    # Sync changes to assets
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/Blueprints     ./Content
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/HUDAssets      ./Content
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/Models         ./Content
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/StarterContent ./Content
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/VehicleAdv     ./Content
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/Weather        ./Content

    set +x
    popd >/dev/null
fi
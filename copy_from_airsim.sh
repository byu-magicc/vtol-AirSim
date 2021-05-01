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
if [[ -z "$(git status --porcelain=v1 2>/dev/null)" ]]; then
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    pushd "$SCRIPT_DIR" >/dev/null

    set -x
    # Sync changes to AirLib source files
    rsync -a --delete $AIRSIMPATH/AirLib/include  Source/AirLib
    rsync -a --delete $AIRSIMPATH/AirLib/src      Source/AirLib

    # Sync changes to AirSim plugin source files
    rsync -a --delete $AIRSIMPATH/Unreal/Plugins/AirSim/Source          ./
    rsync -a --delete $AIRSIMPATH/Unreal/Plugins/AirSim/AirSim.uplugin  ./

    # Sync changes to assets
    rsync -a --delete $AIRSIMPATH/Unreal/Plugins/AirSim/Content ./

    set +x
    popd >/dev/null
else
    echo ""
    echo "Please commit/stash your changes in 'vtol-AirSim-Plugin' before running this script!"
    echo ""
    echo "    That way, if anything goes wrong, you can easily see what changed."
    echo ""
    echo ""
    exit 1
fi
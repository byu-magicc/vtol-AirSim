if [[ -z "${AIRSIMPATH}" ]]; then
    echo ""
    echo "AIRSIMPATH not set."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
fi

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null
rsync -a --delete Source/AirLib/include $AIRSIMPATH/AirLib
rsync -a --delete Source/AirLib/src     $AIRSIMPATH/AirLib

rsync -a --delete Source/*.h    $AIRSIMPATH/Unreal/Plugins/AirSim/Source
rsync -a --delete Source/*.cpp  $AIRSIMPATH/Unreal/Plugins/AirSim/Source

rsync -a --delete Source/Recording \\
                  Source/SimHUD \\
                  Source/SimJoyStick \\
                  Source/SimMode  \\
                  Source/UnrealSensors \\
                  Source/Vehicles \\
                  Source/Weather \\
                  $AIRSIMPATH/Unreal/Plugins/AirSim/Source
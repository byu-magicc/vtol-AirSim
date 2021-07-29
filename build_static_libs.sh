#!/bin/bash -e

if [[ -z "${AIRSIMPATH}" ]]; then
    echo ""
    echo "AIRSIMPATH not set."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
elif [[ ! -d "${AIRSIMPATH}/AirLib" ]]; then
    echo ""
    echo "AIRSIMPATH is set to ${AIRSIMPATH}, but AirLib directory was not found at that path."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

# Create a backup of current AirLib source code 
alib_backup="airlib_backup"
backup="$AIRSIMPATH/$alib_backup"

function cleanup {
    if [[ -d "$backup" ]]; then
        echo ""
        echo "Restoring ${alib_backup}/include and ${alib_backup}/src to AirLib/"
        echo ""
        rsync -a --delete "$backup"/include "$AIRSIMPATH"/AirLib
        rsync -a --delete "$backup"/src     "$AIRSIMPATH"/AirLib
        rm -rf "$backup"
    fi
    cd "$SCRIPT_DIR"
}

if [[ -d "$backup" ]]; then
    rm -rf "$backup"
fi
echo ""
echo "Creating backup of AirLib at ${alib_backup}/"
echo ""
mkdir -p "$backup" 
rsync -a "$AIRSIMPATH"/AirLib/include "$backup"
rsync -a "$AIRSIMPATH"/AirLib/src     "$backup"
trap cleanup EXIT

rsync -a --delete Source/AirLib/include "$AIRSIMPATH"/AirLib
rsync -a --delete Source/AirLib/src     "$AIRSIMPATH"/AirLib
pushd "$AIRSIMPATH" >/dev/null

# Download rpclib
RPC_VERSION="2.3.0"
RPC_FOLDER="rpclib-$RPC_VERSION"
if [ ! -d "external/rpclib/$RPC_FOLDER" ]; then
    echo "*********************************************************************************************"
    echo "Downloading rpclib..."
    echo "*********************************************************************************************"

    wget https://github.com/rpclib/rpclib/archive/v${RPC_VERSION}.zip

    # remove previous versions
    rm -rf "external/rpclib"

    mkdir -p "external/rpclib"
    unzip -q v${RPC_VERSION}.zip -d external/rpclib
    rm v${RPC_VERSION}.zip
fi

# Download high-polycount SUV model
if [ ! -d "Unreal/Plugins/AirSim/Content/VehicleAdv" ]; then
    mkdir -p "Unreal/Plugins/AirSim/Content/VehicleAdv"
fi
if [ ! -d "Unreal/Plugins/AirSim/Content/VehicleAdv/SUV/v1.2.0" ]; then
        echo "Downloading high-poly car assets..."
        if [ -d "suv_download_tmp" ]; then
            rm -rf "suv_download_tmp"
        fi
        mkdir -p "suv_download_tmp"
        cd suv_download_tmp
        wget  https://github.com/Microsoft/AirSim/releases/download/v1.2.0/car_assets.zip
        if [ -d "../Unreal/Plugins/AirSim/Content/VehicleAdv/SUV" ]; then
            rm -rf "../Unreal/Plugins/AirSim/Content/VehicleAdv/SUV"
        fi
        unzip -q car_assets.zip -d ../Unreal/Plugins/AirSim/Content/VehicleAdv
        cd ..
        rm -rf "suv_download_tmp"
fi

if [ ! -d "AirLib/deps/eigen3" ]; then
    echo "Downloading Eigen..."
    wget -O eigen3.zip https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.zip
    unzip -q eigen3.zip -d temp_eigen
    mkdir -p AirLib/deps/eigen3
    mv temp_eigen/eigen*/Eigen AirLib/deps/eigen3
    rm -rf temp_eigen
    rm eigen3.zip
fi

echo ""
echo "************************************"
echo "AirLib setup completed successfully"
echo "Begin build"
echo "************************************"

debug=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
    --debug)
        debug=true
        shift # past argument
        ;;
    esac
done

# variable for build output
if $debug; then
    build_dir=build_debug
else
    build_dir=build_release
fi

CMAKE=$(which cmake)
export CC="clang"
export CXX="clang++"

if [[ -f "./cmake/CMakeCache.txt" ]]; then
    rm "./cmake/CMakeCache.txt"
fi
if [[ -d "./cmake/CMakeFiles" ]]; then
    rm -rf "./cmake/CMakeFiles"
fi

if [[ -d "$build_dir" ]]; then
    rm -rf $build_dir
fi

mkdir -p $build_dir
pushd $build_dir  >/dev/null

folder_name=""
if $debug; then
    folder_name="Debug"
else
    folder_name="Release"
fi

echo ""
echo "************************************"
echo "cmake ../cmake -DCMAKE_BUILD_TYPE=${folder_name} --- inside ${build_dir}"
echo "************************************"

"$CMAKE" ../cmake -DCMAKE_BUILD_TYPE="${folder_name}" \
    || (popd && rm -r $build_dir && exit 1)

echo ""
echo "************************************"
echo "make -j$(nproc) AirLib --- inside ${build_dir}"
echo "************************************"

make -j`nproc` AirLib

popd >/dev/null

mkdir -p AirLib/lib/x64/$folder_name
mkdir -p AirLib/deps/rpclib/lib
mkdir -p AirLib/deps/MavLinkCom/lib
cp $build_dir/output/lib/libAirLib.a AirLib/lib
cp $build_dir/output/lib/libMavLinkCom.a AirLib/deps/MavLinkCom/lib
cp $build_dir/output/lib/librpc.a AirLib/deps/rpclib/lib/librpc.a

# Update AirLib/lib, AirLib/deps
rsync -a --delete $build_dir/output/lib/ AirLib/lib/x64/$folder_name
rsync -a --delete external/rpclib/${RPC_FOLDER}/include AirLib/deps/rpclib
rsync -a --delete MavLinkCom/include AirLib/deps/MavLinkCom

popd >/dev/null

# back in vtol-AirSim
rsync -a --delete "$AIRSIMPATH"/AirLib/deps Source/AirLib
rsync -a --delete "$AIRSIMPATH"/AirLib/lib  Source/AirLib

# copy high-poly SUV model (if needed) to avoid warnings in Unreal Editor
if [ ! -d "Content/VehicleAdv/SUV" ]; then
    rsync -a --delete "$AIRSIMPATH"/Unreal/Plugins/AirSim/Content/VehicleAdv/SUV Content/VehicleAdv
fi

popd >/dev/null


echo ""
echo "************************************"
echo "Finished building AirLib"
echo "************************************"

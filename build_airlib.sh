if [[ -z "${AIRSIMPATH}" ]]; then
    echo ""
    echo "AIRSIMPATH not set."
    echo "Please set AIRSIMPATH environment variable to the directory where AirSim is located on your system before running this script."
    echo ""
    exit 1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null
rsync -a --delete Source/AirLib/include $AIRSIMPATH
rsync -a --delete Source/AirLib/src     $AIRSIMPATH
pushd "$AIRSIMPATH" >/dev/null

export C_COMPILER="/usr/bin/clang"
export COMPILER="/usr/bin/clang++"
export CC="/usr/bin/clang"
export CXX="/usr/bin/clang++"

export CMAKE=$(which cmake)

build_dir=build_debug

if [[ ! -d "external/rpclib" ]]; then
    echo "*********************************************************************************************"
    echo "Downloading rpclib..."
    echo "*********************************************************************************************"

    wget  https://github.com/rpclib/rpclib/archive/v2.2.1.zip

    mkdir -p "external/rpclib"
    unzip v2.2.1.zip -d external/rpclib
    rm v2.2.1.zip
fi

echo "### Downloading and installing llvm 5 libc++ library..."

if [[ ! -d "llvm-source-50" ]]; then
    git clone --depth=1 -b release_50  https://github.com/llvm-mirror/llvm.git llvm-source-50
    git clone --depth=1 -b release_50  https://github.com/llvm-mirror/libcxx.git llvm-source-50/projects/libcxx
    git clone --depth=1 -b release_50  https://github.com/llvm-mirror/libcxxabi.git llvm-source-50/projects/libcxxabi
else
    echo "folder llvm-source-50 already exists, skipping git clone..."
fi

if [[ ! -d "llvm-build" ]]; then
    mkdir -p llvm-build
    pushd llvm-build >/dev/null

    echo "*********************************************************************************************"
    echo "cmake --- in llvm-build"
    echo "*********************************************************************************************"


    "$CMAKE" -DCMAKE_C_COMPILER=${C_COMPILER} -DCMAKE_CXX_COMPILER=${COMPILER} \
            -LIBCXX_ENABLE_EXPERIMENTAL_LIBRARY=OFF -DLIBCXX_INSTALL_EXPERIMENTAL_LIBRARY=OFF \
            -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=./output \
                ../llvm-source-50

    echo "*********************************************************************************************"
    echo "make cxx --- in llvm-build"
    echo "*********************************************************************************************"

    make cxx -j`nproc`

    echo "*********************************************************************************************"
    echo "make install-libcxx install-libcxxabi --- in llvm-build"
    echo "*********************************************************************************************"

    make install-libcxx install-libcxxabi

    popd >/dev/null
fi

if [[ ! -d "AirLib/deps/eigen3" ]]; then
    echo "Downloading eigen..."
    wget -O eigen3.zip https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.zip
    unzip eigen3.zip -d temp_eigen
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

echo ""
echo "************************************"
echo "cmake ../cmake -DCMAKE_BUILD_TYPE=Debug --- inside ${build_dir}"
echo "************************************"

"$CMAKE" ../cmake -DCMAKE_BUILD_TYPE=Debug \
    || (popd && rm -r $build_dir && exit 1)

echo ""
echo "************************************"
echo "make -j$(nproc) AirLib --- inside ${build_dir}"
echo "************************************"

make -j`nproc` AirLib
popd >/dev/null


mkdir -p AirLib/lib/x64/Debug
mkdir -p AirLib/deps/rpclib/lib
mkdir -p AirLib/deps/MavLinkCom/lib
cp $build_dir/output/lib/libAirLib.a AirLib/lib
cp $build_dir/output/lib/libMavLinkCom.a AirLib/deps/MavLinkCom/lib
cp $build_dir/output/lib/librpc.a AirLib/deps/rpclib/lib/librpc.a

# Update AirLib/lib, AirLib/deps, Plugins folders with new binaries
rsync -a --delete $build_dir/output/lib/ AirLib/lib/x64/Debug
rsync -a --delete external/rpclib/rpclib-2.2.1/include AirLib/deps/rpclib
rsync -a --delete MavLinkCom/include AirLib/deps/MavLinkCom

popd >/dev/null
rsync -a --delete $AIRSIMPATH/AirLib/deps Source/AirLib
rsync -a --delete $AIRSIMPATH/AirLib/lib  Source/AirLib
popd >/dev/null

echo ""
echo "************************************"
echo "Finished building AirLib"
echo "************************************"

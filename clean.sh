#! /bin/bash

# get path of current script: https://stackoverflow.com/a/39340259/207661
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

set -e
set -x

# clean temporary unreal folders
rm -rf Binaries
rm -rf Intermediate
rm -rf Saved
rm -rf ../../Binaries
rm -rf ../../Intermediate
rm -rf ../../Saved
rm -f  ../../CMakeLists.txt
rm -f  ../../Makefile

popd >/dev/null

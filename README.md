# vtol-AirSim-Plugin
A fork (copy) of [AirSim](https://github.com/microsoft/AirSim)'s Unreal Engine Plugin with added functionality for VTOL aircraft. See also [vtol-AirLib](https://github.com/byu-magicc/vtol-AirLib) - a submodule of this repository located at Source/AirLib.

## Setup Instructions

**This repository uses git LFS.**
1. On Ubuntu 20.04, install it by running 
    ```
    sudo apt install git-lfs
    git lfs install
    ```
    Now use git as you normally would. 
1. Install Unreal Editor 4.24
    - Follow the [guide in the AirSim docs](https://microsoft.github.io/AirSim/build_linux/#linux-build-unreal-engine)
    - **Make sure to checkout the `4.24` branch!**
1. Clone the `byu-magicc/AirSim` repo. By default it will be checked out on the `vtol` branch.
    - `git clone git@github.com:byu-magicc/AirSim.git`
1. Copy the `Blocks` Unreal environment out of the `AirSim` repository
    - `cp -r ./AirSim/Unreal/Environments/Blocks ./`
1. Set the `AIRSIMPATH` environment variable
    - `export AIRSIMPATH=$(pwd)/AirSim`
1. Create a `Plugins` directory in `Blocks`
    - `mkdir ./Blocks/Plugins`
1. Clone this repository into `Blocks/Plugins` and update the submodules to get the `vtol-AirLib` repo
    ```
    cd ./Blocks/Plugins
    git clone git@github.com:byu-magicc/vtol-AirSim-Plugin.git
    cd vtol-AirSim-Plugin
    git submodule update --init --recursive
    ```
1. Build using `build_airlib.sh`
    ```
    cd vtol-AirSim-Plugin
    ./build_airlib.sh
    ```

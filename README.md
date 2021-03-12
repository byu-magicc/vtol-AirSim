# vtol-AirSim-Plugin
A fork (copy) of [AirSim](https://github.com/microsoft/AirSim)'s Unreal Engine Plugin with added functionality for VTOL aircraft. See also [vtol-AirLib](https://github.com/byu-magicc/vtol-AirLib) - a submodule of this repository located at Source/AirLib.

## Setup Instructions

**This repository uses git LFS. Make sure to install it before cloning**

1. Install Unreal Editor 4.24

  - Follow the [guide in the AirSim docs](https://microsoft.github.io/AirSim/build_linux/#linux-build-unreal-engine)
  - **Make sure to checkout the `4.24` branch!**

2. Clone the `byu-magicc/AirSim` repo and checkout the `vtol` branch
  - `git clone -b vtol git@github.com:byu-magicc/AirSim.git`

3. Copy the `Blocks` Unreal environment out of the `AirSim` repository
  - `cp -r ./AirSim/Unreal/Environments/Blocks ./`

4. Set the `AIRSIMPATH` environment variable
  - `export AIRSIMPATH=$(PWD)/AirSim`

5. Create a `Plugins` directory in `Blocks`
  - `mkdir ./Blocks/Plugins`

6. Clone this repository into `Blocks/Plugins` and update the submodules to get the `vtol-AirLib` repo

  - `cd ./Blocks/Plugins`

  - `git clone git@github.com:byu-magicc/vtol-AirSim-Plugin.git`

  - `git submodule update --init --recursive`

  - `git lfs fetch; git lfs checkout`
7. Build using `build_airlib.sh`
  - `./build_airlib.sh`

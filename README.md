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
1. Install Unreal Editor 4.25
    - Follow the [guide in the AirSim docs](https://microsoft.github.io/AirSim/build_linux/#linux-build-unreal-engine)
    - **Make sure to checkout the `4.25` branch!**
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
1. Build static library files using `build_static_libs.sh` (builds rpclib, MavLinkCom, and rpc components of AirLib)
    ```
    cd vtol-AirSim-Plugin
    ./build_static_libs.sh
    ```

Now start up Unreal Editor. To open the Blocks project, go to More -> Browse, navigate to where you put the directory containing the Blocks project and open the file `Blocks.uproject`. When it prompts if you would like to rebuild the missing modules (Blocks and AirSim, which haven't been built yet), select "Yes". 

## Development Workflow
When you have made changes to AirLib or the AirSim plugin code and you want to test your changes in Unreal, you will need to rebuild the entire AirSim plugin and your Unreal project. It's annoying, but this is the way Unreal deals with plugins; any changes to a plugin requires rebuilding from scratch. There isn't a way around this, currently.

### How to Build After Modifying Code

There are two methods you can follow to rebuild the plugin and your project:
1. Run the script `clean.sh` contained in the Blocks project. 
    - You can copy this script to any project you want. 
    - All it does is delete the three folders `Binaries`, `Intermediate`, and `Saved` in both the project folder and in the project's `Plugins/AirSim` folder, so you could also delete those folders manually.
    - When you next open your project in the editor, you will be prompted to rebuild the missing modules (since their build files were deleted). Select "Yes", and it will build everything.
2. Run the following command:
   ```
   <unreal_path>/Engine/Build/BatchFiles/Linux/Build.sh BlocksEditor Linux Development <project_path>/Blocks.uproject -waitmutex
   ```
    - Substitute `<unreal_path>` with the path to where you cloned the Unreal Engine repo, and `<project_path>` with the path to your Unreal project. As an example, the Blocks project is used, but you can swap out `BlocksEditor` and `Blocks.uproject` with your project name. For example, if your project's name is `Yeet`, substitute with `YeetEditor` and `Yeet.uproject`.
    - This runs the build command that the editor runs for you in in the first method.
    - Set this command to an alias for easy reuse (e.g. `ue_build_blocks`). 
    - On my system, this saves ~10 sec of build time compared to the first method.

After finishing either method, start the Unreal Editor and open your project. Your changes should now be reflected when you play in the editor. 

**Tip**: In the Unreal Editor window that first opens for selecting your project, you can click "More" and then check the box "Always load last project on startup", then open your project. This will save you startup time when you initialize the Unreal Editor. 

### Note: a few files require running `build_static_libs.sh` to rebuild
If you modify any of these files, then you will have to build the static libraries again by running the `build_static_libs.sh` script:
- `RpcLibClientBase`, `RpcLibServerBase`
- `FileSystem`
- `ObstacleMap`
- `SafetyEval`
- `CarRpcLibClient`, `CarRpcLibServer`
- `MultirotorRpcLibClient`, `MultirotorRpcLibServer`
- `TiltrotorRpcLibClient`, `TiltrotorRpcLibServer`
- `MultirotorApiBase`, `TiltrotorApiBase`

Run the script **before** you perform one of the two rebuild methods. 
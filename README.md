# vtol-AirSim-Plugin
A fork (copy) of [AirSim](https://github.com/microsoft/AirSim)'s Unreal Engine plugin with added functionality for VTOL aircraft. 

See also [vtol-AirLib](https://github.com/byu-magicc/vtol-AirLib) - a submodule of this repository located at Source/AirLib.

### Quick links

* [Setup](#setup)
* [Development](DEVELOPMENT.md)
* [Maintaining the byu-magicc/AirSim Fork](FORK.md)

## Setup Instructions

**This repository uses Git LFS (Large File Storage).**
1. On Ubuntu 20.04, install Git LFS by running 
    ```
    sudo apt install git-lfs
    git lfs install
    ```
    Now use git as you normally would. 
1. Install the build dependency `clang` by running
    ```
    sudo apt install clang
    ```
    - as of this writing, the `clang` package on Ubuntu 20.04 installs **clang 10**
    - check your version of clang with: `clang --version`, then run:
    ```
    sudo apt install libc++-<version>-dev
    ```
    - where `<version>` is `10` for clang 10, `9` for clang 9, etc.

1. Install Unreal Editor 4.25
    - Follow the ["Linux - Build Unreal Engine" steps in the AirSim docs](https://microsoft.github.io/AirSim/build_linux/#linux-build-unreal-engine)
        - **Make sure you checked out the `4.25` branch before building** 
            - fast way: `git clone -b 4.25 https://github.com/EpicGames/UnrealEngine.git`
1. Clone the `byu-magicc/AirSim` fork repo. You'll be working on the `vtol` branch (the default branch).
    `git clone git@github.com:byu-magicc/AirSim.git`
1. Copy the `Blocks` Unreal environment out of the `AirSim` fork repository
    - `cp -r ./AirSim/Unreal/Environments/Blocks ./`
1. Set `AIRSIMPATH` environment variable - you can add this (using absolute path) to your `~/.bashrc` or `~/.zshrc` to have it set for all future terminal sessions
    - `export AIRSIMPATH="$(pwd)/AirSim"`
1. Create a `Plugins` directory in `Blocks`
    - `mkdir ./Blocks/Plugins`
1. Clone this repository into `Blocks/Plugins` and update the submodules to get the `vtol-AirLib` repo (located at `Source/AirLib`)
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
1. Now start up Unreal Editor. You can find it at the following location (may want to make an alias for this):
    ```
    /<path_to_cloned_unreal_engine_repo>/Engine/Binaries/Linux/UE4Editor
    ```
1. When the editor is up and running, go to More > Browse, navigate to where you put the directory containing the Blocks project and open the file `Blocks.uproject`. 
   - If you get a prompt that says "This project was made with a different version of the Unreal Engine" and asks if you want to open a copy, instead select "More Options..." then choose "Convert in-place".
   - When it prompts if you would like to rebuild the missing modules (Blocks and AirSim, which haven't been built yet), select "Yes". 

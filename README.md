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

### Note: some files require running `build_static_libs.sh` to rebuild
If you modify any of these files/C++ classes, then you will have to build the static libraries again by running the `build_static_libs.sh` script:
- Any file with `*RpcLibClient*` or `*RpcLibServer*` in the name
- `MultirotorApiBase` or `TiltrotorApiBase`
- `SafetyEval`, `ObstacleMap`, or `FileSystem`

Run the script **before** you perform one of the two rebuild methods. 

### Sync your changes to AirSim fork
At some point you may want to copy your modifications of the source code over to [our fork of AirSim](https://github.com/byu-magicc/AirSim). There are a few reasons you may want to do this:
- if there are new commits to AirSim master that you want to merge into our code (see next section for guidance)
- in preparation for creating a pull request to AirSim
- to compare exactly what additions/modifications we have made to the AirSim repo
- to clearly show the overall state of our code, to whomever that may benefit

A convenience script is provided called `sync_to_airsim_repo.sh` for copying your modifications to wherever your cloned AirSim fork is located (assumed to be at `$AIRSIMPATH`). When you run this script, the source code files inside the following directories will be copied to their respective locations in the AirSim repo as follows:
- `vtol-AirSim-Plugin/Source --> $AIRSIMPATH/Unreal/Plugins/AirSim/Source` (excluding `vtol-AirSim-Plugin/Source/AirLib`)
- `vtol-AirSim-Plugin/Content/Tiltrotor --> $AIRSIMPATH/Unreal/Plugins/AirSim/Content/Tiltrotor` (the asset files)
- `vtol-AirSim-Plugin/Source/AirLib --> $AIRSIMPATH/AirLib`

### Merging upstream AirSim commits to the fork, then to your code
If new commits have been pushed to the base AirSim repo and you want to merge those commits into our fork of AirSim, then here is how you can go about doing that. You'll need to make some commits, so make sure your code is in a "presentable" state, i.e. it builds, preferably it has been tested and works, and you've removed your 55 print statements you sprinkled all over the place while debugging.
1. It's a good idea to make a new temporary branch on the AirSim fork. For example, you can make a new branch called `merge-upstream`. To do that, cd into `$AIRSIMPATH`, run `git status` to make sure your fork is checked out on the desired branch and is free of any uncommitted changes, then run:
    - `git checkout -b merge-upstream`
1. Navigate back to `vtol-AirSim`. If your code is in a presentable state, go ahead and run `./sync_to_airsim_repo.sh` to copy your modifications over to the repository at `$AIRSIMPATH`.
1. Go back to `$AIRSIMPATH`. Run `git status` and `git diff` to make sure you didn't just copy over anything crazy.
1. If everything looks good, `git add` all of your changes and commit them on your new branch.
1. Now pull from AirSim master branch. This part can be a little tricky.
   1. If you haven't already, [add the AirSim base repo as a remote](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/configuring-a-remote-for-a-fork). You can add it as a remote with the name of `upstream` by the following command:
      - `git remote add upstream https://github.com/microsoft/AirSim.git`
      - After that, check that it worked with `git remote -v`.
   1. Switch to your master branch: `git checkout master`.
   1. Now run `git pull upstream master`. Your local clone of the AirSim fork should now have all the lastest changes from the base master branch.
   1. Switch back to your temporary branch, i.e.: `git checkout merge-upstream`.
1. Mentally prepare yourself, then run `git merge master`.
1. If you're lucky and there were no merge conflicts, then you'll now have all your changes and all the upstream changes together on your temporary branch!
    - But if you do have conflicts, then sorry, but you will have to manually fix those. Hopefully there aren't too many.
    - Once you've fixed the conflicts and committed the result, then you can celebrate!
1. You can now create a pull request on GitHub to merge your new branch into the `vtol` branch (or whichever branch you're developing on). 
   - Run `git push -u origin merge-upstream` to push your branch to the fork on GitHub.
   - Have someone approve it, then merge it in, then you can delete your temporary branch. 

If you would like to copy the changes introduced by the merge to your `vtol-AirSim-Plugin` and `vtol-AirSim-Plugin/Source/AirLib` directories, then you can run the script `copy_from_airsim.sh`. To protect against overwriting any changes you've made to your files, you must commit and/or stash all changes in your `vtol-AirSim-Plugin` working tree before the script will allow any copying to happen. 
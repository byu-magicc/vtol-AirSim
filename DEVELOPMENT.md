## Development
When you have made changes to AirLib or the AirSim plugin code and you want to test your changes in Unreal, you will need to rebuild the entire AirSim plugin and your Unreal project. It's unfortunate, but this is the way Unreal deals with plugins: any changes to a plugin requires rebuilding from scratch. 

## First Step: Build Static Libraries
**Note: you must run the script `build_static_libs.sh` to build some parts of the code.**

Run `build_static_libs.sh` **first** if you are setting up vtol-AirSim for the first time.

You will also need to re-run the script whenever you modify the `.hpp` or `.cpp` files associated with any of the following C++ classes:
- Any class with `*RpcLibClient*` or `*RpcLibServer*` in the name
- `MultirotorApiBase` or `TiltrotorApiBase`
- `SafetyEval`, `ObstacleMap`, or `FileSystem`

If you have modified any of those files, then run `build_static_libs.sh` **first**, and then perform one of the build methods listed below. 

## Building Your Unreal Project

There are two options to rebuild the plugin and your project:

### Option 1: Delete build directories
Delete the directories `Binaries`, `Intermediate`, and `Saved` in both the project folder **and** the `Plugins/vtol-AirSim` folder (6 directories total).

You can use the script `clean.sh` contained in the Blocks project if you modify lines 14-16 to have the path `Plugins/vtol-AirSim/*` instead of `Plugins/AirSim/*`.

When you next open your project in the editor, you will be prompted to rebuild the missing modules (because their build files were deleted). Select "Yes", and it will build your project and the plugin.

### Option 2: Run a command
```
<unreal_path>/Engine/Build/BatchFiles/Linux/Build.sh BlocksEditor Linux Development <project_path>/Blocks.uproject -waitmutex
```
- Substitute `<unreal_path>` with the path to your clone of the Unreal Engine repo, and `<project_path>` with the path to your Unreal project. 
- The above command is for the Blocks project, but you can swap out `BlocksEditor` and `Blocks.uproject` with your project name. For example, if your project's name is `Yeet`, substitute those values with `YeetEditor` and `Yeet.uproject`.
- This runs the build command that the editor normally runs for you when using the first method.
- Set this command to an alias for easy reuse (e.g. `ue_build_blocks`). 
- On my system, this saves ~10 sec of build time compared to the first method.

After finishing either method, start the Unreal Editor and open your project. Your changes should now be reflected when you play the sim in the editor. 

**Tip**: In the Unreal Editor window that first opens for selecting your project, you can click "More" and then check the box "Always load last project on startup", then open your project. This will save you startup time when you initialize the Unreal Editor. 

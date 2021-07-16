## Development
When you have made changes to AirLib or the AirSim plugin code and you want to test your changes in Unreal, you will need to rebuild the entire AirSim plugin and your Unreal project. It's unfortunate, but this is the way Unreal deals with plugins: any changes to a plugin requires rebuilding from scratch. 

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
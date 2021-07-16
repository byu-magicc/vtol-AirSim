# Maintaining the Fork: [byu-magicc/AirSim](https://github.com/byu-magicc/AirSim)

### Why have 3 repositories instead of one fork of AirSim?
The way that AirSim has set their repo up is cumbersome for development. It is a large, monolithic repo where interrelated code is scattered around in places that do not reflect where the code needs to be at compile time, thus making the reading, writing, and testing of code awkward.

For example, when building the Blocks project - let's say to build it within the repo at `AirSim/Unreal/Environments/Blocks` - the directory `AirSim/Unreal/Plugins/AirSim` needs to be located under `Blocks/Plugins/AirSim`, meaning it must be copied over. But that's not all, the `AirSim` plugin (as in the AirSim plugin for Unreal Engine) needs to have `AirSim/AirLib` located at `AirSim/Unreal/Plugins/AirSim/Source/AirLib`, meaning both needs to be copied into `Blocks/Plugins/AirSim`. Once you've done that, then Unreal Engine is happy and VSCode (or your favorite editor/IDE) can find everything, but when it comes time to commit your changes, you have to copy the source files back to their original locations (with your changes). 

That didn't make sense to some of us, and we wanted to simply write code and commit changes as we made them instead of having to remember to copy back and forth all the time. So, we made two repositories - one for AirLib, and one for the AirSim plugin - where development of the Unreal Engine code (vtol-AirSim-Plugin) could be separated from the physics, sensor, comms, etc. code (vtol-AirLib). 

### OK, so then why have a fork? 
For several reasons. Most of them have to do with the way git - and GitHub - work. Having a fork allows us to:

- use git to do the complicated task of merging new commits from AirSim master into our fork, which can then be merged into our code with a single copy-and-commit 
- create pull requests to AirSim when we want to contribute changes
- compare exactly what additions/modifications we have made to the AirSim repo
- clearly display a snapshot of our code, to whomever that may benefit

## Syncing changes to vtol-AirSim-Plugin and vtol-AirLib to the AirSim fork
A convenience script is provided called `sync_to_airsim_repo.sh` for copying your modifications to wherever your cloned AirSim fork is located (assumed to be at `$AIRSIMPATH`). When you run this script, the source code files inside the following directories will be copied to their respective locations in the AirSim repo as follows:
- `vtol-AirSim-Plugin/Source --> $AIRSIMPATH/Unreal/Plugins/AirSim/Source` (excluding `vtol-AirSim-Plugin/Source/AirLib`)
- `vtol-AirSim-Plugin/Content/Tiltrotor --> $AIRSIMPATH/Unreal/Plugins/AirSim/Content/Tiltrotor` (the asset files)
- `vtol-AirSim-Plugin/Source/AirLib --> $AIRSIMPATH/AirLib`

## Merging upstream commits from microsoft/AirSim to byu-magicc/AirSim
If new commits have been pushed to the base AirSim repo and you want to merge those commits into our fork of AirSim, then here is how you can do that. You'll need to make some commits, so make sure your code is in a "presentable" state, i.e. it builds, preferably it has been tested and works, and you've removed your 55 print statements you sprinkled all over the place while debugging.
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

## Copying from the AirSim fork to vtol-AirSim-Plugin and vtol-AirLib
If you would like to copy the changes introduced by the merge to your `vtol-AirSim-Plugin` and `vtol-AirSim-Plugin/Source/AirLib` directories, then you can run the script `copy_from_airsim.sh`. To protect against overwriting any changes you've made to your files, you must commit and/or stash all changes in your `vtol-AirSim-Plugin` working tree before the script will allow any copying to happen. 
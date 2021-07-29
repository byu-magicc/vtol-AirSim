# Maintaining the Fork: [byu-magicc/AirSim](https://github.com/byu-magicc/AirSim)

### Why have 2 repositories instead of just one fork of AirSim?
The layout of the AirSim repository is cumbersome for development. It is a large monorepo where interdependent code is scattered around in places that do not reflect where the code needs to be at compile time, and this makes the reading, writing, and testing of the AirSim source code awkward.

For example, when building the Blocks project - let's say to build it within the repo at `Unreal/Environments/Blocks` - the directory `Unreal/Plugins/AirSim` needs to be located under `Unreal/Environments/Blocks/Plugins/AirSim`, meaning it must be copied over. In addition, the *AirSim* plugin - as in the plugin for Unreal Engine named *AirSim* - needs to have `AirSim/AirLib` located at `Unreal/Plugins/AirSim/Source/AirLib`, meaning both directories need to be copied into `Blocks/Plugins/AirSim`. Once you've done that, then Unreal Engine is happy and your editor/IDE can find everything, but when it comes time to commit your changes, you have to copy the source files back to their original locations (with your changes). 

That didn't make sense to some of us, and we wanted to simply write code and commit changes as we made them instead of having to remember to copy back and forth all the time. So, we made two repositories - one for development (this one), and one to be a literal fork of AirSim.

### OK, then why have a fork? 
For several reasons. Most of them have to do with the way git - and GitHub - work. Having a fork allows us to:

- use git to do the complicated task of merging new commits from AirSim master into our fork, which can then be merged into our code with a single copy-and-commit 
- create pull requests to AirSim when we want to contribute changes
- compare exactly what additions/modifications we have made to the AirSim repo
- clearly display a snapshot of our code, to whomever that may benefit

Now, instead of having to copy files around every time you want to commit your changes, you can simply commit your changes when you make them, and you only occasionally need to copy files from or to the AirSim fork.

## Updating the AirSim Fork With Changes to vtol-AirSim
A convenience script is provided called `sync_to_airsim_repo.sh` which copies your modifications to `$AIRSIMPATH` - which should be set to where you cloned the byu-magicc/AirSim repo. 

When you run this script, the source code files inside the following directories will be copied to their respective locations in the AirSim repo:
- `vtol-AirSim-Plugin/Source --> $AIRSIMPATH/Unreal/Plugins/AirSim/Source` 
   - excluding `vtol-AirSim-Plugin/Source/AirLib`
- `vtol-AirSim-Plugin/Content/Tiltrotor --> $AIRSIMPATH/Unreal/Plugins/AirSim/Content/Tiltrotor` 
- `vtol-AirSim-Plugin/Source/AirLib --> $AIRSIMPATH/AirLib`

## Merging Upstream Commits From microsoft/AirSim to byu-magicc/AirSim
If new commits have been pushed to the base AirSim repo and you want to merge those commits into our fork of AirSim, then here is a guide on how to do that.

You'll need to make some commits, so make sure your code is in a "presentable" state, i.e. you've removed the 55 debugging print statements that you sprinkled all over the place, it builds, and (preferably) it has been tested and works.
1. Recommended: make a new temporary branch on the AirSim fork. 
   - For example, you can make a new branch called `merge-upstream`. To do so:
      - cd into `$AIRSIMPATH`
      - run `git status` to make sure your fork is on the desired branch and is free of any uncommitted changes
      - run `git switch -c merge-upstream`
1. Navigate to your local `vtol-AirSim` repo. If your code is in a "presentable" state, run the script `./sync_to_airsim_repo.sh` to copy your modifications over to `$AIRSIMPATH`.
1. Go back to `$AIRSIMPATH`. Run `git status` and `git diff` to make sure you didn't just copy over anything crazy.
1. If everything looks good, `git add` all of your changes and commit them on your new branch.
1. Pull from AirSim master branch.
   1. If you haven't already, add the AirSim base repo as a remote. You can add it as a remote with the name of `upstream` by running the following command:
      - `git remote add upstream https://github.com/microsoft/AirSim.git`
      - Check that it worked with `git remote -v`.
      - for more info on remotes and forks, see [this GitHub doc.](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/configuring-a-remote-for-a-fork)
   1. Switch to master: `git switch master`.
   1. Now run `git pull upstream master`. Your local AirSim fork's master branch should now have all the lastest changes from the base AirSim master.
   1. Switch back to your temporary branch: `git switch merge-upstream`.
1. **Mentally prepare yourself for the merge.**
1. Run `git merge master`.
   - If you're lucky and there were no merge conflicts, then you'll now have all your changes and all the upstream changes together on your new branch!
   - If you do have merge conflicts, then sorry, but you will have to manually fix those. Hopefully there aren't too many.
      - Once you've fixed the conflicts and committed the result, then you can celebrate!
1. Create a pull request on GitHub to merge your temporary branch into the main branch of byu-magicc/AirSim.
   - Be sure to run `git push -u origin merge-upstream` to push your branch to GitHub.
   - Have someone approve the pull request then merge it in. 
   - After that, you can delete your temporary branch. 

### After the Merge: Copying From the AirSim Fork to vtol-AirSim
To copy the changes introduced by the merge to `vtol-AirSim`, run the script `copy_from_airsim.sh`

The script has a safety measure in place to prevent overwriting any of your uncommitted changes: you must commit or stash all changes in your `vtol-AirSim` working tree before the script will do any copying of files.
# MegaMan X4 Practice
This is a hack to assist with practicing speedruns of the PSX version of MegaMan X4. It primarly adds save states which are not normally possible on PSX games due to the nature of the Disc Games.

## Compile
NOTE these steps might change in the future so keep that in mind when fetching the newest version of this repro.
1st your gonna need a total of 3 repros (including this one)
 * Red-Hot's PSX Modding Repro https://github.com/mateusfavarin/psx-modding-toolchain
 * My MMX4 PSX Repro https://github.com/Kuumba123/MegaManX4_PS1_Modding

 Also your gonna need the psyq library files (`psyq-4.7-converted-full.7z`) from https://psx.arthus.net/sdk/Psy-Q/

Once you have all 3 go back to the github with Red-Hot's PSX modding repro and following the instructions there. If you can compile any of the examples included in that repro you should be able to compile this one.

Then put the MegaMan X4 PSX repro inside the `games` folder and then this repro inside your `mods` folder and put your MegaMan X4 ISO files inside the `build` folder (must be named `MMX4.cue` and `MMX4.bin` or `RMX4.cue` and `RMX4.bin`).

Once your done setting up all the files go back to the MMX4 Practice repro and run the MOD.BAT file. The only options that you should really be interested in are the option `1` , `3` and `4`. 1st you should do option 3 to extract the game files for editing then option 1 to compile then option 4 to actually build the game disc. Once you have run option 4 and you should see the new ISO files in the build folder.
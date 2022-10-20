# osu-aim-torture-maker

This is a program to help making aim training maps for osu! that will force your muscle memory to (re-)learn how to move from circle to circle in any possible direction on the playfield.

All the circles are placed on the borders of the playfield for maximum pleasure.

# Example mapset

https://osu.ppy.sh/beatmapsets/1533708

# How to use

1. Create a map with a song that has static bpm
2. Find the correct offset and bpm
3. Open this program and input them here
4. Type in how precise you want to train your muscle memory/aim horizontally and vertically. More points - more precise
5. (OPTIONAL) Specify random seed
6. Wait a bit and get your hitcircle data in hitobjects.txt that you can copypaste into the map
7. pls enjoy aim

# How it works?

Initially it chooses all the circles on the borders of the playfield depending on how many points horizontally and vertically you typed in e.g. for 8 horizontal points osu! playfield horizontal border is divided by 8 -> 512 / 8 -> 64. Every 64 osu! units on the playfield on the border there will be a circle used for generating the map: 0:0, 64:0, 128:0, 192:0, 256:0, 320:0, 384:0, 448:0, 512:0 at the top of the playfield and 0:384, 64:384, 128:384 e.t.c. at the bottom of the playfield

Then it finds all possible paths between circles. (from each circle to another one in both directions)

And lastly it makes one single **random** sequence of circles that follows all the paths and **only once** for each path.

Paths that follow the same straight line are filtered out e.g. 0:32 to 0:64 and 0:0 to 0:384 are on the same line so only 0:0 to 0:384 and backwards is used.

osu! playfield is 512x384 osu! units

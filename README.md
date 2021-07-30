# osu-aim-torture-maker

This is a program to help making aim training maps for osu! that will force your muscle memory to (re-)learn how to move from circle to circle in any possible direction on the playfield.

All the circles are placed on the borders of the playfield for maximum pleasure.

# Example mapset

https://osu.ppy.sh/beatmapsets/1533708

# How to use

1. Create a map with a song that has static bpm
2. Find the correct offset and bpm
3. Open this program and input them here
4. Type in the step value 128 or 64 or 32. It decides how precise you want to train your muscle memory/aim. Step 128 / 64 / 32 correspond to total 126 / 536 / 2232 generated circles
5. Wait a bit and get your hitcircle data in hitobjects.txt that you can copypaste into the map
6. pls enjoy aim

# How it works?

Initially it chooses all the circles on the borders of the playfield fitting precision step criteria e.g. for step 128 circles at 0:0, 0:128, 0:256, 0:384, 128:0, 256:0 e.t.c are chosen.

Then it finds all possible paths between circles. (from each circle to another one in both directions)

And lastly it makes one single **random** sequence of circles that follows all the paths and **only once** for each path.

Paths that follow the same straight line are filtered out e.g. 0:32 to 0:64 and 0:0 to 0:384 are on the same line so only 0:0 to 0:384 and backwards is used.

osu! playfield is 512x384 osu! units

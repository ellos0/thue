#!/usr/bin/env thue
#fractal tree art L system in thue

#0 -> draw a line segment ending in a leaf
#1 -> draw a line segment
#[ -> push position and angle. left 45 degrees
#] -> push position and angle. right 45 degrees

1::=11
0::=1[0]0

::=
0

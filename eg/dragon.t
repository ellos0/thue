#!/usr/bin/env thue
#the dragon curve using an L system.

#F -> draw forward
#G -> draw forward
#+ -> turn left
#- -> turn right

F::=F+G
G::=F-G

::=

F

Hexagon tile flipping puzzle (source: unknown!)

Apparently the puzzle is as follows. Take a hexagon of size 4 (diameter 7) and
color tiles red or blue. Red tiles are worth nothing. Blue tiles are worth 2
points plus 1 point per red neighbor. What is the maximum total score you can
achieve?

Try it out here: https://maksverver.github.io/hexagon-puzzle/

Index:

  brute-force-size-3.cc
  brute-force-size-4.cc

    Solve the problem by brute force for hexagons of size 3 and 4.

  dynamic-programming-size-4.cc

    Calculates the maximum attainable score (but no solution!)

  best-solution-size-3.txt
  best-solution-size-4.txt

    Contain the best solution for hexagons of size 3 and 4.
    (Solutions are equivalent up to rotation/reflection.)

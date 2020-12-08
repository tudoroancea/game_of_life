# Game of Life emulator

Based on John Conway's "Game of Life" cellular automaton

## Current Features :
Simple "Game of Life" simulation on a grid of max dimension 400x400.

## Future Features :
Usage of pre-existing cell patterns.
Informations on the simulation (number of generation, speed of the simulation…)

## Compiling :
In the project root directory execute
```
cmake
cmake --build
```
After compiling the project with either CMake (using the CMakeLists.txt) or qmake (using the .pro), you are asked the dimensions of the grid.
Once the dimensions selected, the grd appears and you can click on the cell you want to make alive. Clicking a second time on them kills them.
You can then start the simulation using the "lancer" button in the top left corner.
At any point in the simulation you can pause it using the "pause" button in the top right corner, and resume using the same button. While paused, the grid can be updated using the same mecanism as before.

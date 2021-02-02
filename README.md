# Game of Life emulator

This open-source GUI emulator based on John Conway's "Game of Life" cellular automaton is just a way of enhancing our C++ (and more generally programming) skills. This whole project is still ongoing and not stable at all. We purposely don't give some clear instructions on the usage because nothing is still settled. All criticism is welcome !\
Out of laziness (of writing complicated parsers), we used some simple powerful pre-existing libraries : rapidcsv (https://github.com/d99kris/rapidcsv) and Termcolor (https://github.com/ikalnytskyi/termcolor.git).

## What our program does :
The main program is a simple "Game of Life" graphic simulation on a 400x400 grid (with 50 cells of margin on each side to avoid boundary effects). When the simulation is paused, you can use the mouse to add and delete cells, either one by one or inserting whole structures (presaved or via copy/paste). You can also zoom in/out and translate the grid to see a specific part of the simulation.

## What our program doesn't :
- The zoom is EXTREMELY unstable when using a trackpad or mouses without wheels (typically Apple Magic Mouses).
- Currently you can't display presaved simulations (although there are dialog boxes on the first window saying otherwise).
- Always enter some valid number for the dimensions of the grid (otherwise there will be no grid :| )
- In the simulation source files there is a Doxygen style documentation of each class and method, even if there is not any Doxygen generator support


## Future Features :
- Stable zoom
- Further informations on the simulation (number of cells, expansion speed, number of generations per second,…)
- Display of presaved simulations

## How to compile (on UNIX machines):
First make sure you have installed the Qt library (version 5.14 or more recent) and CMake (version 3.7 or more recent). Then download the project using :
```
git clone https://github.com/tudoroancea/game_of_life
cd game_of_life
```
Once in the project directory, compile using CMake ;
```
mkdir build
cmake -B build -DCMAKE_PREFIX_PATH=Qt5
cmake --build build --target main
```
where Qt5 represents the path to the file "Qt5Config.cmake" or "qt5-config.cmake". Execute the main program using :
```
./build/src/main
```
To build and execute the different tests replace the last 2 steps by
```
cmake --build build --target test_XXX
./build/test/test_XXX
```

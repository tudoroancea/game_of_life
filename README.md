# Game of Life emulator

Hello there !

This open-source GUI emulator of [John Conway's "Game of Life"](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) 
has two objectives :
1) *To a lesser extent* : popularize this cellular automaton
2) *To a bigger extent* : enhance our C++ (and more generally programming) skills by solving concrete problems.
    This is for both of us a first experience with such a large project and we want to make it look
    as professional as possible, as close to an actual modern software engineering project as possible.
    That is we don't only code a simple program, we also try to include unit tests, CI/CD, use a more complex git branching system, etc.

I insist, none of us has real experience on the subject, we learn as we go. All remarks and constructive criticism are more than welcome. 😇
## What our program does :
The main program named **GOL** is a simple graphic simulation of the Game of Life. The default size of the grid is 400x400 (with 50 cells of margin on each side to avoid boundary effects).

There are basically 3 functionning states : Selecting, Adding, and Deleting.
1) *Selecting* : You can double click to create a selection area and select living cells. Double clicking again will define a new rectangle and try to extend the selection area.\
Then you can copy these cells and paste them wherever you want. Once pasted, the cells are not yet inserted in the grid, you still have to press the **Return** or **Enter** key to insert it.
2) *Adding* : You can draw new cells
3) *Deleting* : You can delete cells

There are several convenient features already implemented :
| Feature                     | Shortcurt                         | Also available in      |
|-----------------------------|-----------------------------------|------------------------|
| Pause/Resume the simulation | Space bar                         | Tool bar and View Menu |
| Zoom in                     | Ctrl + on Window / Cmd + on MacOS  | Tool bar and View Menu |
| Zoom Out                    | Ctrl - on Windows / Ctrl + on MacOS | Tool bar and View Menu |
| Reset zoom level            | Ctrl 0 on Window / Cmd 0 on MacOS   | Tool bar and View Menu |
| Vertical Scroll             | Mouse Wheel /  Vertical scroll on trackpad | Tool bar and View Menu |
| Horizontal Scroll           | Shift Mouse Wheel /  Horizontal scroll on trackpad | Tool bar and View Menu |
| Undo                        | Ctrl Z on Windows / Cmd Z on MacOS  | Tool bar and Edit Menu |
| Copy                        | Ctrl C on Windows / Cmd C on MacOS  | Edit Menu |
| Cut                         | Ctrl X on Windows / Cmd X on MacOS  | Edit Menu |
| Paste                       | Ctrl V on Windows / Cmd V on MacOS  | Edit Menu |
| Clear the whole grid | Ctrl K on Windows / Cmd K on MacOS  | Edit Menu |


\
\
There are also some programs in the subdirectory `test` which were only tools for us to test specific features.
They are NOT unit tests (yet).

## What our program doesn't do (yet, but hopefully will at some point):
- Save what the cells you copied
- Save a whole simulation
- Display previously saved cells motives or simulations
- Right click menu for copy/cut/paste or similar
- Multi-language support
- Change/Display simulation speed
- Dark mode for MacOS

## How to compile (on UNIX machines):
First make sure you have installed the Qt library (version 5.14 or more recent) and CMake (version 3.7 or more recent). Then download the project using :
```
git clone https://github.com/tudoroancea/game_of_life
cd game_of_life
```
Once in the project directory, compile using CMake ;
```
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target GOL
```
If neither `Qt5Config.cmake` nor `qt5-config.cmake` files are available using your `PATH` environment variable, add the flag `-DCMAKE_PREFIX_PATH=Qt5` to the second line.

Then execute the main program using :
```
./build/src/main
```
To build and execute the different tests replace the last 2 steps by
```
cmake --build build --target test_XXX
./build/test/test_XXX
```

## How to help us:
If you test out program (either by building from sources or using pre-compiled releases) and something doesn't work, please tell us.\
In particular tell us (depending on your case) your OS and OS Version, CMake and Qt Version.
Thanks a lot!
## Copyright :
Our whole project is licenced under the MIT (permissive) licence.

Out of laziness (of writing complicated parsers and stuff), we used some simple powerful pre-existing header-only libraries ([rapidcsv](https://github.com/d99kris/rapidcsv) and [Termcolor](https://github.com/ikalnytskyi/termcolor.git))
as well as [Google Test](https://github.com/google/googletest) for the unit testing framework.
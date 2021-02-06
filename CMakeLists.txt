cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Game_of_life VERSION 1.0
                     LANGUAGES CXX)

# Sets the default build mode on debug
if(NOT CMAKE_BUILD_TYPE)
        #set(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")
        set(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "" FORCE)
endif()

# C++ standard
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Warnings for Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
# Speed optimization for Release mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Chemins des dossiers du projet
add_definitions(-DTEST_DEF)
add_definitions(-DGLOBAL_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DSRC_PATH="${CMAKE_SOURCE_DIR}/src")
add_definitions(-DSIMULATION_PATH="${CMAKE_SOURCE_DIR}/src/simulation")
add_definitions(-DQT_PATH="${CMAKE_SOURCE_DIR}/src/qt")
add_definitions(-DCLI_PATH="${CMAKE_SOURCE_DIR}/src/cli")
add_definitions(-DTEST_PATH="${CMAKE_SOURCE_DIR}/test")
add_definitions(-DDATA_PATH="${CMAKE_SOURCE_DIR}/data")
add_definitions(-DLOCAL_PATH="${CMAKE_SOURCE_DIR}/data/local")
add_definitions(-DPRESAVED_PATH="${CMAKE_SOURCE_DIR}/data/presaved")

# Diverses définitions pré-processeur utiles
# Dimensions of a GameOfLife grid (excluding the 50 cells margins on each side to avoid boundary effects)
add_definitions(-DMAX_LIGNES=400)
add_definitions(-DMAX_COLONNES=400)

# Comment the following lines depending on the debug information you need
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Leave uncommented if you want to know whenever GameOfLife(View)::add_cell() or suppr_cell() or inv_cell() or resize() are called on too big coordinates
    add_definitions(-DOVERFLOW_WARNINGS)

    # Leave uncommented if you want to know whenever
    #add_definitions(-DALREADY_MODIFIED_WARNINGS) GameOfLife(View)::add_cell() or suppr_cell() or inv_cell() are called on already modified cells

    # Leave uncommented if you want to know when we try to browse non existing paths on the computer
    add_definitions(-DNON_EXISTING_PATH_WARNINGS)

    # Leave uncommented if you want to know if we try to load a simulation saved with different dimensions than the current version of the prograù
    add_definitions(-DWRONG_DIMENSIONS_WARNINGS)
endif()


# Verbose for extra compile information
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ajoute les librairies et executables créés dans src et test
add_subdirectory(src)
add_subdirectory(test)

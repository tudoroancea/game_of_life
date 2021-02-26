cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(game_of_life VERSION 1.0
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
add_compile_definitions(GLOBAL_PATH="${CMAKE_SOURCE_DIR}")
add_compile_definitions(SRC_PATH="${CMAKE_SOURCE_DIR}/src")
add_compile_definitions(SIMULATION_PATH="${CMAKE_SOURCE_DIR}/src/simulation")
add_compile_definitions(QT_PATH="${CMAKE_SOURCE_DIR}/src/qt")
add_compile_definitions(CLI_PATH="${CMAKE_SOURCE_DIR}/src/cli")
add_compile_definitions(TEST_PATH="${CMAKE_SOURCE_DIR}/test")
add_compile_definitions(DATA_PATH="${CMAKE_SOURCE_DIR}/data")
add_compile_definitions(LOCAL_PATH="${CMAKE_SOURCE_DIR}/data/local")
add_compile_definitions(PRESAVED_PATH="${CMAKE_SOURCE_DIR}/data/presaved")

# Some useful pre-processor definitions ================================================================================================
# Dimensions of a GameOfLife grid (excluding the 50 cells margins on each side to avoid boundary effects)
add_compile_definitions(MAX_LIGNES=400)
add_compile_definitions(MAX_COLONNES=400)

# Comment the following lines depending on the debug information you need
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Leave uncommented if you want to know whenever GameOfLife(View)::addCell() or deleteCell() or inverseCell() or resize() are called on too big coordinates
    #add_definitions(-DOVERFLOW_WARNINGS)
    add_compile_definitions(OVERFLOW_WARNINGS)

    # Leave uncommented if you want to know whenever GameOfLife(View)::addCell() or deleteCell() or inverseCell() are called on already modified cells
    #add_compile_definitions(ALREADY_MODIFIED_WARNINGS)

    # Leave uncommented if you want to know when we try to browse non existing paths on the computer
    add_compile_definitions(NON_EXISTING_PATH_WARNINGS)

    # Leave uncommented if you want to know if we try to load a simulation saved with different dimensions than the current version of the prograù
    add_compile_definitions(WRONG_DIMENSIONS_WARNINGS)

    #Leave uncommented if you want the methods of the class Simulation to throw errors
    #    add_compile_definitions(THROW_SIMULATION_ERRORS)
endif()

#include(CTest)
#enable_testing()

# Verbose for extra compile information
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ajoute les librairies et executables créés dans src et test
add_subdirectory(src)
add_subdirectory(test)

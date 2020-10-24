cmake_minimum_required(VERSION 3.17)
project(Game_of_life)

# Sets the default build mode on debug
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "" FORCE)
endif()
# C++ standard
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
# Warnings for Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
# Speed optimization for Release mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Verbose for extra compile information
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
add_definitions( -DGLOBAL_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ajoute les librairies et executables créés dans src et test
add_subdirectory(src)
add_subdirectory(test)

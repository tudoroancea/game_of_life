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

# Verbose for extra compile information
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ajoute les librairies et executables créés dans src et test
add_subdirectory(src)
add_subdirectory(test)

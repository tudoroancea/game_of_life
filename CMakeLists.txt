cmake_minimum_required(VERSION 3.17)
project(Game_of_life)

# For extra information
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
endif()

add_subdirectory(src)

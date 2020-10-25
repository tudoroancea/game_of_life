set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

add_definitions(-DGLOBAL_PATH="{CMAKE_SOURCE_DIR}")
add_definitions(-DSRC_PATH="$${CMAKE_SOURCE_DIR}/src")
add_definitions(-DSIMULATION_PATH="$${CMAKE_SOURCE_DIR}/src/simulation")
add_definitions(-DQT_PATH="$${CMAKE_SOURCE_DIR}/src/qt")
add_definitions(-DCLI_PATH="${CMAKE_SOURCE_DIR}/src/cli")
add_definitions(-DTEST_PATH="${CMAKE_SOURCE_DIR}/test")
add_definitions(-DLOCAL_PATH="${CMAKE_SOURCE_DIR}/data/local")
add_definitions(-DPRESAVED_PATH="${CMAKE_SOURCE_DIR}/data/presaved")


#permet de trouver les bibliotheques
add_subdirectory(qt)
add_subdirectory(simulation)
add_subdirectory(cli)

# Permet les includes sans prefix
include_directories(simulation qt cli)

add_executable(main main.cpp)
target_link_libraries(main LINK_PUBLIC qt simulation)
target_link_libraries(main PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui)
#target_include_directories(main PUBLIC simulation qt)

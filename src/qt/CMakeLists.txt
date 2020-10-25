set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

add_library(qt
    mainwindow.h cell_.h
    mainwindow.cpp cell_.cpp
)

# permet les includes sans prefix
include_directories(../simulation)

target_link_libraries(qt LINK_PUBLIC simulation)
target_link_libraries(qt PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

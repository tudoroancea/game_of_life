set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

# Chemins des dossiers du projet
add_definitions(-DGLOBAL_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DSRC_PATH="${CMAKE_SOURCE_DIR}/src")
add_definitions(-DSIMULATION_PATH="${CMAKE_SOURCE_DIR}/src/simulation")
add_definitions(-DQT_PATH="${CMAKE_SOURCE_DIR}/src/qt")
#add_definitions(-DQT_PATH=".")
add_definitions(-DCLI_PATH="${CMAKE_SOURCE_DIR}/src/cli")
add_definitions(-DTEST_PATH="${CMAKE_SOURCE_DIR}/test")
add_definitions(-DDATA_PATH="${CMAKE_SOURCE_DIR}/data")
add_definitions(-DLOCAL_PATH="${CMAKE_SOURCE_DIR}/data/local")
add_definitions(-DPRESAVED_PATH="${CMAKE_SOURCE_DIR}/data/presaved")

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

add_library(qt
    STATIC
    mainwindow.h cell_.h
    mainwindow.cpp cell_.cpp
)

# permet les includes sans prefix
include_directories(../simulation)

target_link_libraries(qt PUBLIC simulation)
target_link_libraries(qt PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

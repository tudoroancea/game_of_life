set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../simulation)

add_definitions(-DGLOBAL_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DSRC_PATH="${CMAKE_SOURCE_DIR}/src")
add_definitions(-DSIMULATION_PATH="${CMAKE_SOURCE_DIR}/src/simulation")
add_definitions(-DQT_PATH="${CMAKE_SOURCE_DIR}/src/qt")
add_definitions(-DCLI_PATH="${CMAKE_SOURCE_DIR}/src/cli")
add_definitions(-DTEST_PATH="${CMAKE_SOURCE_DIR}/test")
add_definitions(-DDATA_PATH="${CMAKE_SOURCE_DIR}/data")
add_definitions(-DLOCAL_PATH="${CMAKE_SOURCE_DIR}/data/local")
add_definitions(-DPRESAVED_PATH="${CMAKE_SOURCE_DIR}/data/presaved")

add_library(cli
    STATIC
    affichage.h affichage.cpp
)
target_link_libraries(cli LINK_PUBLIC simulation)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(simulation
    game_of_life.cpp game_of_life.h
    motifs.cpp motifs.h
)
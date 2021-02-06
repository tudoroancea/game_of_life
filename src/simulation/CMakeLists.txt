set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_library(simulation
    STATIC
    game_of_life.cpp game_of_life.h
    motifs.cpp motifs.h
    equivalence_table.hpp equivalence_table.cpp
    rapidcsv.h
    termcolor.hpp
)
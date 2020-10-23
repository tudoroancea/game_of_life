set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../simulation)

add_library(cli
    affichage.h affichage.cpp
)
target_link_libraries(cli LINK_PUBLIC simulation)
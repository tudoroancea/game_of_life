cmake_minimum_required(VERSION 3.17)
project(Game_of_life)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

add_executable(main
        main.cpp
        cell_.h
        game_of_life.h
        mainwindow.h
        motifs.h
        cell_.cpp
        game_of_life.cpp
        mainwindow.cpp
        motifs.cpp
)
add_executable(time_test
        time_test.cpp
        game_of_life.cpp game_of_life.h
        motifs.h motifs.cpp
)

target_link_libraries(main PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

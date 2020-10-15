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
        src/main.cpp
        src/qt/cell_.h
        src/simulation/game_of_life.h
        src/qt/mainwindow.h
        src/simulation/motifs.h
        src/qt/cell_.cpp
        src/simulation/game_of_life.cpp
        src/qt/mainwindow.cpp
        src/simulation/motifs.cpp
)
add_executable(time_test
        test/time_test.cpp
        src/simulation/game_of_life.cpp src/simulation/game_of_life.h
        src/simulation/motifs.h src/simulation/motifs.cpp
)

target_link_libraries(main PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

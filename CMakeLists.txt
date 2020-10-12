cmake_minimum_required(VERSION 3.17)
project(game_of_life)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(SRC
        cell_.cpp
        game_of_life.cpp
        mainwindow.cpp
        moc_mainwindow.cpp
        motifs.cpp
)
set(HDR
        cell_.h
        game_of_life.h
        mainwindow.h
        moc_mainwindow.h
        motifs.h
)

add_executable(main main.cpp SRC HDR)

target_link_libraries(main PRIVATE)
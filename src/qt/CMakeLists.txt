set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Pour faire marcher Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)


add_library(qt
    STATIC
    mainwindow.h cell_.h
    mainwindow.cpp cell_.cpp
)

# permet les includes sans prefix
include_directories(../simulation)

target_link_libraries(qt PUBLIC simulation)
target_link_libraries(qt PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

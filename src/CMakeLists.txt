set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

add_definitions( -DSRC_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
#permet de trouver les bibliotheques
add_subdirectory(qt)
add_subdirectory(simulation)
add_subdirectory(cli)

# Permet les includes sans prefix
include_directories(simulation qt cli)

add_executable(main main.cpp)
target_link_libraries(main LINK_PUBLIC qt simulation)
target_link_libraries(main PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui)
#target_include_directories(main PUBLIC simulation qt)

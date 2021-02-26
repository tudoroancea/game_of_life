set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Pour faire marcher Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)


#permet de trouver les bibliotheques et targets créées
add_subdirectory(qt)
add_subdirectory(simulation)
add_subdirectory(cli)

# Permet les includes sans prefix
include_directories(simulation qt cli)

add_executable( main
        main.cpp
        qt/resources.qrc
)
#target_link_libraries(main LINK_PUBLIC )
target_link_libraries(main PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui qt simulation)

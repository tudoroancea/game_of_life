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

if (APPLE)
#	set_target_properties(GOL PROPERTIES MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons8-seed_of_life.png)

	# NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
	# the property added to Info.plist
	set(MACOSX_BUNDLE_ICON_FILE icon.icns)
#	set(MACOSX_BUNDLE_ICON_FILE "")
	# And this part tells CMake where to find and install the file itself
	set(APP_ICON_MACOSX icon.icns)
	set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	add_executable(GOL MACOSX_BUNDLE main.cpp qt/images.qrc ${APP_ICON_MACOSX})
	add_custom_command(TARGET GOL POST_BUILD COMMAND plutil -replace NSHighResolutionCapable -bool true GOL.app/Contents/Info.plist)

else()
	add_executable(GOL main.cpp qt/images.qrc)
endif()

target_link_libraries(GOL PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui qt simulation)

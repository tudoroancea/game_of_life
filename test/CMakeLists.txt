set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS -pthread)

project(gol_tests)
include(FetchContent)
FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

include_directories(../src/simulation ../src/qt ../src/cli)

include(GoogleTest)

# Unit tests ====================================================================================
add_executable(golUnitTest golUnitTest.cpp)
target_link_libraries(golUnitTest LINK_PUBLIC simulation)
target_link_libraries(golUnitTest PUBLIC gtest gtest_main)
gtest_discover_tests(golUnitTest)

add_executable(motifUnitTest motifUnitTest.cpp)
target_link_libraries(motifUnitTest LINK_PUBLIC simulation)
target_link_libraries(motifUnitTest PUBLIC gtest gtest_main)
gtest_discover_tests(motifUnitTest)

# Normal tests ====================================================================================
add_executable(test_divers test_divers.cpp)
target_link_libraries(test_divers LINK_PUBLIC qt simulation)

add_executable(test_movablegroup test_movablegroup.cpp)
target_link_libraries(test_movablegroup PUBLIC qt simulation Qt5::Widgets Qt5::Gui Qt5::Core)
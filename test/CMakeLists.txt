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
string(REPLACE "-Wundef" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

# Unit tests ====================================================================================
add_executable(unittest unittest.cpp)
target_link_libraries(unittest PUBLIC gtest gtest_main)
gtest_discover_tests(unittest)

add_executable(golUnitTest golUnitTest.cpp)
target_link_libraries(golUnitTest LINK_PUBLIC simulation)
target_link_libraries(golUnitTest PUBLIC gtest gtest_main)
gtest_discover_tests(golUnitTest)

add_executable(motifUnitTest motifUnitTest.cpp)
target_link_libraries(motifUnitTest LINK_PUBLIC simulation)
target_link_libraries(motifUnitTest PUBLIC gtest gtest_main)
gtest_discover_tests(motifUnitTest)

# Normal tests ====================================================================================
add_executable(time_test time_test.cpp)
target_link_libraries(time_test LINK_PUBLIC qt simulation)

add_executable(test_segment test_segment.cpp)
target_link_libraries(test_segment LINK_PUBLIC simulation)

add_executable(test_save_motif test_save_motif.cpp)
target_link_libraries(test_save_motif LINK_PUBLIC simulation)


add_executable(test_rotation test_rotation.cpp)
target_link_libraries(test_rotation LINK_PUBLIC simulation)

add_executable(test_save_sim test_save_sim.cpp)
target_link_libraries(test_save_sim LINK_PUBLIC simulation)

add_executable(test_cli test_cli.cpp)
target_link_libraries(test_cli LINK_PUBLIC simulation cli)

add_executable(test_connexite test_connexite.cpp)
target_link_libraries(test_connexite LINK_PUBLIC simulation)

add_executable(test_resize test_resize.cpp)
target_link_libraries(test_resize LINK_PUBLIC simulation)

add_executable(test_gameoflifeview test_gameoflifeview.cpp)
target_link_libraries(test_gameoflifeview LINK_PUBLIC simulation)

add_executable(test_existing_sims test_existing_sims.cpp)
target_link_libraries(test_existing_sims LINK_PUBLIC simulation)

add_executable(test_rapidcsv test_rapidcsv.cpp)
target_link_libraries(test_rapidcsv LINK_PUBLIC simulation)

add_executable(test_color test_color.cpp)
target_link_libraries(test_color LINK_PUBLIC simulation)

add_executable(test_equivalence_table test_equivalence_table.cpp)
target_link_libraries(test_equivalence_table LINK_PUBLIC simulation)

add_executable(test_divers test_divers.cpp)
target_link_libraries(test_divers LINK_PUBLIC qt simulation)

add_executable(test_movablegroup test_movablegroup.cpp)
target_link_libraries(test_movablegroup PUBLIC qt simulation Qt5::Widgets Qt5::Gui Qt5::Core)
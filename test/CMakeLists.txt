set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS -pthread)

project(gol_tests)
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

include_directories(../src/simulation ../src/qt ../src/cli)

# Unit tests ====================================================================================
add_executable(unittest unittest.cpp)

# Normal tests ====================================================================================
add_executable(time_test time_test.cpp)
target_link_libraries(time_test LINK_PUBLIC qt simulation)

add_executable(test_segment test_segment.cpp)
target_link_libraries(test_segment LINK_PUBLIC simulation)

add_executable(test_save_motif test_save_motif.cpp)
target_link_libraries(test_save_motif LINK_PUBLIC simulation)

add_executable(unittest unittest.cpp)
target_link_libraries(unittest LINK_PUBLIC simulation)
add_test(unit unittest)

add_executable(test_rotation test_rotation.cpp)
target_link_libraries(test_rotation LINK_PUBLIC simulation)

add_executable(test_save_sim test_save_sim.cpp)
target_link_libraries(test_save_sim LINK_PUBLIC simulation)

#if(${CMAKE_CXX_COMPILER} MATCHES ${Clang} OR ${CMAKE_CXX_COMPILER} MATCHES ${Linux})
add_executable(test_cli test_cli.cpp)
target_link_libraries(test_cli LINK_PUBLIC simulation cli)
#endif()

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
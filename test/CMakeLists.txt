set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Google Test
#include(GoogleTest)
#add_subdirectory(googletest)

include_directories(../src/simulation ../src/qt)

add_executable(time_test time_test.cpp)
target_link_libraries(time_test LINK_PUBLIC qt simulation)

add_executable(test_segment test_segment.cpp)
target_link_libraries(test_segment LINK_PUBLIC simulation)

add_executable(test_save_motif test_save_motif.cpp)
target_link_libraries(test_save_motif LINK_PUBLIC simulation)

add_executable(unittest unittest.cpp)
target_link_libraries(unittest LINK_PUBLIC simulation)
#target_link_libraries(unittest ${GTEST_BOTH_LIBRARIES} pthread)
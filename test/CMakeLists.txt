set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef -Wuninitialized")
endif()
include_directories(../src/simulation ../src/qt)

add_executable(time_test time_test.cpp)
target_link_libraries(time_test LINK_PUBLIC qt simulation)

add_executable(test_segment test_segment.cpp)
target_link_libraries(test_segment LINK_PUBLIC simulation)
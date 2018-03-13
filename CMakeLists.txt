cmake_minimum_required(VERSION 3.5)
project(clutil)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_STANDARD 11)
add_subdirectory(clutil)

add_executable(test_v1 test_v1.cc)
target_link_libraries(test_v1 clutil)

cmake_minimum_required(VERSION 3.5)

find_package(OpenCL REQUIRED)
message(STATUS "OPENCL_LIB:" ${OpenCL_LIBRARY})
message(STATUS "OPENCL_INCLUDE:" ${OpenCL_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_STANDARD 11)

set(SOURCES
        common_cl.h
        common_cl_compile_program.c
        common_cl_pick_device.c
        common_cl_create_context.c 
		btnc_seq_gen.c
		)
add_subdirectory(clog)

set(LIBS ${OpenCL_LIBRARY} clog ${CMAKE_THREAD_LIBS_INIT})

add_library(clutil ${SOURCES})
target_link_libraries(clutil ${LIBS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/CL_CODE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "COPIED CL SRC")

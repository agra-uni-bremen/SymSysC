cmake_minimum_required(VERSION 2.8.12)
project(SymSysC)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


subdirs(sim)
subdirs(bench)

add_executable(testbench_plic
        main.cpp
)

target_link_libraries(testbench_plic sim bench)
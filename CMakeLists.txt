cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project(SymSysC NONE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


if(BUILD STREQUAL "bytecode")
	enable_language(BYTECODE)
else() #native
	enable_language(CXX)
endif()


subdirs(sim)
subdirs(bench)

add_executable(testbench_plic
        bench_plic.cpp
)
target_link_libraries(testbench_plic sim bench)

add_executable(testbench_sensor
        bench_sensor.cpp
)
target_link_libraries(testbench_sensor sim bench)


add_custom_target(run_klee_tests
    #COMMAND time klee --libcxx --libc=uclibc -posix-runtime llvm_stuffed.bc
    COMMAND time klee --only-output-states-covering-new testbench_plic
    DEPENDS testbench_plic
)

add_custom_target(run_klee_tests_with_instructions
    COMMAND time klee --debug-print-instructions=src:file testbench_plic
    DEPENDS testbench_plic
)
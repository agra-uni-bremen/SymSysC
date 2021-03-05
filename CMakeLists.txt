cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# "-fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -fno-omit-frame-pointer -g3 -O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project(SymSysC NONE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


if(BUILD STREQUAL "bytecode")
    enable_language(BYTECODE)
    MESSAGE("Building LLVM BYTECODE for Klee")
else() #native
    enable_language(CXX)
    MESSAGE("Building native C/C++")
endif()

find_package( SystemC REQUIRED)

include_directories(.)  #klee_conf.h
subdirs(minikernel)
subdirs(bench)


add_executable(testbench_plic
        bench_plic.cpp
)
target_link_libraries(testbench_plic minikernel bench_translated)

add_executable(testbench_vanilla_plic
        bench_plic_vanilla.cpp
)
target_include_directories(testbench_vanilla_plic PRIVATE ${SystemC_INCLUDE_DIR})
target_link_libraries(testbench_vanilla_plic ${SystemC_LIBRARIES} bench_vanilla)

add_executable(testbench_plic_fault
        bench_plic.cpp
)
target_compile_definitions(testbench_plic_fault PRIVATE EXTRA_BUGS)
target_link_libraries(testbench_plic_fault minikernel bench_translated)

add_executable(testbench_sensor
        bench_sensor.cpp
)
target_link_libraries(testbench_sensor minikernel bench_translated)

add_executable(testbench_sensor2
        bench_sensor2.cpp
)
target_link_libraries(testbench_sensor2 minikernel bench_translated)

add_executable(testbench_uart
        bench_uart.cpp
)
target_link_libraries(testbench_uart minikernel bench_translated)

set(KLEE_ARGS 
	"--libcxx"
	"--libc=uclibc"
	"-posix-runtime"
	#"--emit-all-errors=1"
)

add_custom_target(run_klee_tests_with_instructions
    COMMAND time klee ${KLEE_ARGS} --debug-print-instructions=src:file testbench_plic
    DEPENDS testbench_plic
)

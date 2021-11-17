cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# "-fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -fno-omit-frame-pointer -g -O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project(SymSysC NONE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

find_package( SystemC REQUIRED)
set(UCLIBC_TARGET_TRIPLET x86_64-pc-linux-gnu)	#uclibc is built in this target triple

message("Build is set to: ${BUILD}")
if(BUILD STREQUAL "wllvm")
    MESSAGE("Building LLVM BYTECODE for Klee with wllvm")
    enable_language(CXX)
    set(ENV{LLVM_COMPILER} [clang])		#dunno if this works, just call export LLVM_COMPILER=clang
    set(CMAKE_CXX_COMPILER "wllvm++")
    set(CMAKE_C_COMPILER "wllvm")
    set(CMAKE_C_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(CMAKE_CXX_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(SystemC_LIBRARIES ${SystemC_CXX_LIBRARIES})		# as long as it has been built with wllvm...
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--unresolved-symbols=ignore-in-object-files")	# for klee_int() and konsorten
elseif(BUILD STREQUAL "bytecode")
    MESSAGE("Building LLVM BYTECODE for Klee")
    enable_language(BYTECODE)
    set(CMAKE_BYTECODE_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(SystemC_LIBRARIES /home/klee/source/systemc-dist/lib_llvm/libsystemc.so)
else() #native
    enable_language(CXX)
    MESSAGE("Building native C/C++")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
    set(SystemC_LIBRARIES ${SystemC_CXX_LIBRARIES})
endif()
message("Using Systemc: ${SystemC_LIBRARIES}")

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
target_compile_definitions(testbench_vanilla_plic PRIVATE EXTRA_BUGS)
target_link_libraries(testbench_vanilla_plic ${SystemC_LIBRARIES} bench_vanilla)
#target_link_libraries(testbench_vanilla_plic ${SystemC_LIBRARIES})

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

add_custom_target(run_vanilla_bench
    COMMAND time klee -link-llvm-lib=../source/systemc-dist/lib_llvm/libsystemc.so --libcxx testbench_vanilla_plic
    DEPENDS testbench_vanilla_plic
)
 

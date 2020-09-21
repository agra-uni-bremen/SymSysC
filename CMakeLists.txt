cmake_minimum_required(VERSION 2.8.12)
project(SymSysC)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_COMPILER "clang++-9")
set(CMAKE_C_COMPILER "clang-9")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(BYTECODE_CXX_FLAGS -v -flto -c -Xclang -disable-O0-optnone)


subdirs(sim)
subdirs(bench)

add_executable(testbench_plic
        main.cpp
)
target_link_libraries(testbench_plic sim bench)


add_library( klee SHARED IMPORTED) # Is GLOBAl needed?
set_target_properties( klee PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/klee/include")
set_target_properties( klee PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/klee/build/lib/libkleeSupport.a")

add_executable(testbench_plic_llvm
        main.cpp
)
target_compile_options(testbench_plic_llvm PUBLIC "${BYTECODE_CXX_FLAGS}")
target_link_libraries(testbench_plic_llvm klee sim_llvm bench_llvm)



#language llvm bytecode?
cmake_minimum_required(VERSION 2.8.12)

project(SymSysC CXX C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

set(BYTECODE_CXX_FLAGS -DUSE_KLEE -emit-llvm -flto -c -Xclang -disable-O0-optnone)
#set(BYTECODE_LINKER_FLAGS -v -fuse-ld=lld -flto)-Wl,--unresolved-symbols=ignore-in-object-files


subdirs(sim)
subdirs(bench)

add_executable(testbench_plic
        main.cpp
)
set_target_properties(testbench_plic PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(testbench_plic sim bench)


# As library, so no premature linking
add_library(testbench_plic_llvm
        main.cpp
)
target_compile_options(testbench_plic_llvm PUBLIC "${BYTECODE_CXX_FLAGS}")
#target_link_options(testbench_plic_llvm PUBLIC "${BYTECODE_LINKER_FLAGS}" -Wl,--unresolved-symbols=ignore-in-object-files)
target_link_libraries(testbench_plic_llvm sim_llvm bench_llvm)


add_custom_command(#TARGET testbench_plic_llvm POST_BUILD
    COMMAND llvm-link $$\(find -iname *.o | grep llvm | xargs echo\) -o llvm_stuffed.bc
    DEPENDS testbench_plic_llvm
    OUTPUT llvm_stuffed.bc
)

add_custom_target(run_klee_tests
    #COMMAND time klee --libcxx --libc=uclibc -posix-runtime llvm_stuffed.bc
    COMMAND time klee --only-output-states-covering-new llvm_stuffed.bc
    DEPENDS llvm_stuffed.bc
)

add_custom_target(run_klee_tests_with_instructions
    COMMAND time klee --debug-print-instructions=src:file llvm_stuffed.bc
    DEPENDS llvm_stuffed.bc
)


# clang -flto -std=c++14 -stdlib=libc++ ./CMakeFiles/testbench_plic_llvm.dir/main.cpp.o ./sim/CMakeFiles/sim_llvm.dir/sysc/kernel/sc_time.cpp.o ./sim/CMakeFiles/sim_llvm.dir/tlm_utils/simple_target_socket.cpp.o ./sim/CMakeFiles/sim_llvm.dir/registry.cpp.o ./bench/CMakeFiles/bench_llvm.dir/plic.cpp.o -o testbench_plic_llvm
#language llvm bytecode?
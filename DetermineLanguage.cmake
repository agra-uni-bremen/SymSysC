# This file is intended to be "included", not "include_directories()".
# This is to enable the language switch from a different file than the root file.

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package( SystemC REQUIRED)
set(UCLIBC_TARGET_TRIPLET x86_64-pc-linux-gnu)	#uclibc is usually built in this target triple
find_program(CLANG "clang" REQUIRED)

message("Build is set to: ${BUILD}")
if(BUILD STREQUAL "wllvm")
    MESSAGE("Building LLVM BYTECODE for Klee with wllvm")
    set(ENV{LLVM_COMPILER} [clang])		#dunno if this works, just call `export LLVM_COMPILER=clang`
    set(CMAKE_CXX_COMPILER "wllvm++")
    set(CMAKE_C_COMPILER "wllvm")
    set(CMAKE_C_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(CMAKE_CXX_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(SystemC_LIBRARIES ${SystemC_wllvm_BYTECODE_LIBRARIES})
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--unresolved-symbols=ignore-in-object-files")	# for klee_int() and konsorten
    enable_language(CXX)
elseif(BUILD STREQUAL "bytecode")
    MESSAGE("Building LLVM BYTECODE for Klee")
    find_program(LLVM_LINKER "llvm-link" REQUIRED)
    find_program(KLEE "klee" REQUIRED)
    find_program(LIBS_FOR_libc++ "g++" REQUIRED)
    
    set(CMAKE_BYTECODE_COMPILER_TARGET ${UCLIBC_TARGET_TRIPLET})
    set(SystemC_LIBRARIES ${SystemC_llvm_BYTECODE_LIBRARIES})
    enable_language(BYTECODE)

else() #native
    enable_language(CXX)
    MESSAGE("Building native C/C++")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
    set(SystemC_LIBRARIES ${SystemC_CXX_LIBRARIES})
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--unresolved-symbols=ignore-in-object-files")	# for klee_int() and konsorten
endif()

message("Using SystemC from ${SystemC_LIBRARIES}")

if(NOT EXISTS ${SystemC_LIBRARIES})
	message("${SystemC_LIBRARIES} does not exist, calling ugly buildscript")
	execute_process ( COMMAND ${CMAKE_CURRENT_LIST_DIR}/build_sysc_all.sh
                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ RESULT_VARIABLE ret )
	if(ret AND NOT ret EQUAL 0)
		message ( WARNING "Could not build SystemC, expect errors" )
	endif()
endif()
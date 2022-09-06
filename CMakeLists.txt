cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# "-fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -fno-omit-frame-pointer -g -O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

#project(SymSysC NONE)

# for klee_conf.h, to be included even in non-minikernel envs
include_directories(./klee)

add_subdirectory(minikernel)
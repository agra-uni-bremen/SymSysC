file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(bench
		${SOURCES}
        ${HEADERS}
)
add_library(bench_llvm
		${SOURCES}
        ${HEADERS}
)
target_compile_options(bench_llvm PUBLIC "${BYTECODE_CXX_FLAGS}")
#target_link_options(   bench_llvm PUBLIC "${BYTECODE_LINKER_FLAGS}")

target_include_directories(bench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bench_llvm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( bench sim )
target_link_libraries( bench_llvm sim_llvm )
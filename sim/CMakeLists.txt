file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(sim
		${SOURCES}
        ${HEADERS}
)
add_library(sim_llvm
		${SOURCES}
        ${HEADERS}
)
#target_compile_options(sim_llvm PUBLIC "${BYTECODE_CXX_FLAGS}")

target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(sim_llvm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(bench_translated
        ${SOURCES}
        ${HEADERS}
)


add_library(bench_vanilla
        ${SOURCES}
        ${HEADERS}
)

target_include_directories( bench_translated PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories( bench_vanilla PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories( bench_vanilla PRIVATE ${SystemC_INCLUDE_DIR})

target_link_libraries( bench_translated minikernel )
target_link_libraries( bench_vanilla ${SystemC_LIBRARIES} )

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(bench
		${SOURCES}
        ${HEADERS}
)

target_include_directories(bench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( bench sim )

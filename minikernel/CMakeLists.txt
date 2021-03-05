file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(minikernel
		${SOURCES}
        ${HEADERS} systemc
)
target_include_directories(minikernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)



add_library(sim
		${SOURCES}
        ${HEADERS}
)
        
        
target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
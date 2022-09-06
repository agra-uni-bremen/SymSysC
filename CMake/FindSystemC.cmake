set(SystemC_DIR "${CMAKE_CURRENT_LIST_DIR}/../systemc-dist")

if(NOT EXISTS ${SystemC_DIR})
	execute_process ( COMMAND ${CMAKE_CURRENT_LIST_DIR}/build_sysc_all.sh
                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ ) 
endif()

set(SystemC_INCLUDE_DIRS ${SystemC_DIR}/include )
set(SystemC_INCLUDE_DIR  ${SystemC_INCLUDE_DIRS} )
set(SystemC_CXX_LIBRARIES ${SystemC_DIR}/lib-native/libsystemc.a )
set(SystemC_BYTECODE_LIBRARIES ${SystemC_DIR}/lib-llvm/libsystemc.so )

if (NOT DEFINED SystemC_DIR)
	set(SystemC_DIR "${CMAKE_BINARY_DIR}/systemc-dist")
endif()

set(SystemC_INCLUDE_DIRS ${SystemC_DIR}/include )
set(SystemC_INCLUDE_DIR  ${SystemC_INCLUDE_DIRS} )
set(SystemC_CXX_LIBRARIES ${SystemC_DIR}/lib-native/libsystemc.a )

set(SystemC_llvm_BYTECODE_LIBRARIES ${SystemC_DIR}/lib-llvm/libsystemc.so.bc ) # see build_sysc_all.sh. It is important that it ends with bc.
set(SystemC_wllvm_BYTECODE_LIBRARIES ${SystemC_DIR}/lib-wllvm/libsystemc.a.bc )
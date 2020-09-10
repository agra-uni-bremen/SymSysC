//#include <tlm_utils/simple_target_socket.h>
#pragma once
#include <functional>
#include <vector>

#include "tlm_gp.h"
#include "sc_time.h"
#include "sc_module.h"



#define SC_HAS_PROCESS( shit ) ;


typedef void Transportfunction(tlm::tlm_generic_payload& , sc_core::sc_time&);


namespace sim_tlm_utils
{
	template <class T>
	struct simple_target_socket
	{
		/**
		 * Should somehow register this function to transports singleton
		**/
		void register_b_transport(T& c, Transportfunction& fn);
	};
	
};

#define SYM_THREAD( fun ) threads.append(fun)

std::vector<std::function<Transportfunction>> transports;
std::vector<std::function<void(void)>> threads;
std::vector<std::function<void(void)>> processes;

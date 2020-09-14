#pragma once

#include <tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h>
#include <sysc/kernel/sc_time.h>
#include <registry.hpp>


namespace tlm_utils
{
	template< class MODULE>
	struct simple_target_socket
	{
		void register_b_transport(MODULE* c, void (MODULE::*cb)(tlm::tlm_generic_payload&,
                sc_core::sc_time&))
		{
			transports.push_back(std::bind(cb, c, std::placeholders::_1, std::placeholders::_2));
		};
	};
}

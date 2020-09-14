#pragma once

#include "tlm_gp.h"
#include "sc_time.h"

typedef void Transportfunction(tlm::tlm_generic_payload& , sc_core::sc_time&);

namespace tlm_utils
{
	template< class MODULE>
	struct simple_target_socket
	{
		/**
		 * TODO
		 * Should somehow register this function to transports singleton
		**/
		void register_b_transport(MODULE* c, void (MODULE::*cb)(tlm::tlm_generic_payload&,
                sc_core::sc_time&));
	};
}

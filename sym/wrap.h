#include <tlm_utils/simple_target_socket.h>

#include <functional>


typedef void(tlm::tlm_generic_payload& , sc_core::sc_time&) Transportfunction;

namespace sim_tlm_utils
{
	template <T>
	struct simple_target_socket
	{
		/**
		 * Should somehow register this function to transports singleton
		**/
		void register_b_transport(T* c, Transportfunction);
	};
	
};

#define SC_THREAD(fun) \
#  ;something to call singleton and register function?

std::function<Transportfunction>* transports;
std::function<void(void)>* threads;
std::function<void(void)>* processes;


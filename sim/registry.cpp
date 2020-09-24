#include "registry.hpp"
#include "klee_conf.h"


std::vector<std::function<Transportfunction>> transports;


void minikernel_step()
{
	INFO(std::cout << "running all waiting threads" << std::endl);
	sc_core::Simcontext::get().runNextStep();
}

std::string minikernel_current_time()
{
    return sc_core::Simcontext::get().getGlobalTime().to_string();
}

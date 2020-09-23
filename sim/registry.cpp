#include "registry.hpp"
#include "klee_conf.h"


std::vector<std::function<Transportfunction>> transports;


void run_all_threads()
{
	INFO(std::cout << "running all waiting threads" << std::endl);
	sc_core::Simcontext::get().runNextStep();
}

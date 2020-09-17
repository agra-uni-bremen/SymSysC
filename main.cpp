#include <iostream>

#include "bench/plic.h"
#include "sim/registry.hpp"

int main()
{

	PLIC<1, 64, 32> dut("DUT");

	std::cout << "Number of registered transports: " << transports.size() << std::endl;
	std::cout << "                      processes: " << processes.size() << std::endl;
	std::cout << "                        threads: " << threads.size() << std::endl;


	run_all_threads();



	for(auto i = 1; i < 10; i++)
	{
		dut.gateway_trigger_interrupt(i);
		run_all_threads();
		run_all_threads();
	}

	assert(dut.pending_interrupts[0] > 0);

	return 0;
}

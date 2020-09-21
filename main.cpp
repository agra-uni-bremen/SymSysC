#include <iostream>
#include <klee/klee.h>

#include "bench/plic.h"
#include "sim/registry.hpp"

int main()
{

	PLIC<1, 64, 32> dut("DUT");

	std::cout << "Number of registered transports: " << transports.size() << std::endl;
	std::cout << "                      processes: " << processes.size() << std::endl;
	std::cout << "                        threads: " << threads.size() << std::endl;


	run_all_threads();

	uint32_t i,
	klee_make_symbolic(&i, sizeof(uint32_t), "interrupt number");

	dut.gateway_trigger_interrupt(i);
	run_all_threads();
	run_all_threads();

	assert(dut.pending_interrupts[0] > 0);

	return 0;
}

#include "bench/sensor.h"
#include "sim/registry.hpp"
#include "sim/klee_conf.h"


struct test_interrupt_gateway : interrupt_gateway{
	bool triggered_irq = 0;
	void gateway_trigger_interrupt(uint32_t irq_id){
		triggered_irq = irq_id;
		INFO(std::cout << "interrupt " << irq_id << " fired." << std::endl;);
	}
};


int main()
{
	uint32_t interrupt = klee_int();
	SimpleSensor dut(interrupt);
	test_interrupt_gateway tig;
	dut.plic = &tig;

	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();

	minikernel_step();

	minikernel_step();

	assert(interrupt == tig.triggered_irq);

	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	return 0;
}

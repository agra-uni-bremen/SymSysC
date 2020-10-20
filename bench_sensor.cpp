#include "bench/sensor.hpp"
#include "sim/registry.hpp"
#include "sim/klee_conf.h"


struct test_interrupt_gateway : interrupt_gateway{
	uint32_t triggered_irq = 0;
	void gateway_trigger_interrupt(uint32_t irq_id){
		triggered_irq = irq_id;
		INFO(std::cout << "interrupt " << irq_id << " fired." << std::endl;);
	}
};




int main()
{
	uint32_t interrupt = klee_int("Interrupt");
	SimpleSensor dut(interrupt);
	test_interrupt_gateway tig;
	dut.plic = &tig;

	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();


	minikernel_step();	// 0
	minikernel_step();	// 40ms

	assert(interrupt == tig.triggered_irq);

	sc_core::sc_time delay;
    tlm::tlm_generic_payload pl;
    uint32_t address = klee_int("address");
    uint32_t length = klee_int("length");
    klee_assume(length < INT32_MAX);
    pl.set_read();
    pl.set_address(address);
    pl.set_data_length(length);
    uint8_t* buffer = new uint8_t[INT32_MAX];
    pl.set_data_ptr(buffer);

    dut.transport(pl, delay);



	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	return 0;
}

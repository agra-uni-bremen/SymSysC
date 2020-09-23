#include "bench/plic.h"
#include "sim/registry.hpp"
#include "sim/klee_conf.h"

struct Simple_interrupt_target : public external_interrupt_target
{
    bool was_triggered = false;
    bool was_cleared = false;
    void trigger_external_interrupt()
    {
        INFO(std::cout << "Interrupt triggered" << std::endl);
        was_triggered = true;
    };
    void clear_external_interrupt()
    {
        INFO(std::cout << "Interrupt cleared" << std::endl);
        was_cleared = true;
    }
};


int main()
{
    Simple_interrupt_target it;
	PLIC<1, 64, 32> dut("DUT");
	dut.target_harts[0] = &it;


	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();

	run_all_threads();

	uint32_t i = klee_int("interrupt number");

	//i = 1;

	dut.gateway_trigger_interrupt(i);

	assert(dut.pending_interrupts[0] > 0);

	run_all_threads();

	assert(it.was_triggered);

	return 0;
}

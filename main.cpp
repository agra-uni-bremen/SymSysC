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
    sc_core::sc_time a(1002, sc_core::SC_MS), b (1, sc_core::SC_SEC), c (1, sc_core::SC_FS);
    INFO(std::cout << a.to_string() << " + " << b.to_string() << " = " << (a+b).to_string() << std::endl);
    INFO(std::cout << c.to_string() << " in default units is " << c.to_default_time_units() << " " << sc_core::unit_to_string(sc_core::default_time_unit) << std::endl);

    Simple_interrupt_target it;
	PLIC<1, 64, 32> dut("DUT");
	dut.target_harts[0] = &it;


	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();

	minikernel_step();

	uint32_t i = klee_int("interrupt number");

	i = 1;

	dut.gateway_trigger_interrupt(i);

	assert(dut.pending_interrupts[0] > 0);

	minikernel_step();

	assert(it.was_triggered);

	minikernel_step();
	minikernel_step();

	return 0;
}

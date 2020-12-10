#include "bench/plic.hpp"
#include "sim/registry.hpp"
#include "sim/klee_conf.h"

//todo more flexible?
static constexpr uint32_t numberInterrupts = 64, maxPriority = 32;

struct Simple_interrupt_target : public external_interrupt_target
{
    bool was_triggered = false;
    bool was_cleared = false;
    PLIC<1, numberInterrupts, maxPriority>& dut;

    Simple_interrupt_target(PLIC<1, numberInterrupts, maxPriority>& dut) : dut(dut){};

    void trigger_external_interrupt()
    {
        INFO(std::cout << "Interrupt triggered" << std::endl);
        was_triggered = true;
    };

    void clear_external_interrupt()
    {
        INFO(std::cout << "Interrupt cleared" << std::endl);
        was_cleared = true;
    };

    void claim_interrupt()
    {
        if(was_triggered)
        {
            sc_core::sc_time delay;
            tlm::tlm_generic_payload pl;
            uint32_t interrupt = 0;
            //0x200004 + n*8, &hart_claim_response[n]});
            pl.set_read();
            pl.set_address(0x200004);   //claim_response register
            pl.set_data_length(sizeof(uint32_t));
            pl.set_data_ptr(reinterpret_cast<unsigned char*>(&interrupt));

            dut.transport(pl, delay);

            //If the interrupt was triggered, there has to be an interrupt in register
            assert(interrupt > 0);

            INFO(std::cout << "Interrupt " << interrupt << " claimed" << std::endl);
        }
    }
};


int main()
{
    sc_core::sc_time a(1002, sc_core::SC_MS), b (1, sc_core::SC_SEC), c (1, sc_core::SC_FS);
    INFO(std::cout << a.to_string() << " + " << b.to_string() << " = " << (a+b).to_string() << std::endl);
    INFO(std::cout << c.to_string() << " in default units is " << c.to_default_time_units() << " " << sc_core::unit_to_string(sc_core::default_time_unit) << std::endl);


	PLIC<1, numberInterrupts, maxPriority> dut("DUT");
    Simple_interrupt_target sit(dut);

	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;


	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();

	minikernel_step();

	uint32_t i = klee_int("i interrupt number");

	klee_assume(i < numberInterrupts);

    dut.gateway_trigger_interrupt(i);

    //Is the pending interrupts register changed?
    if(i < 32)
        assert(dut.pending_interrupts[0] > 0);
    else
        assert(dut.pending_interrupts[1] > 0);

    minikernel_step();

    //the step should trigger an external interrupt
    assert(sit.was_triggered);
    assert(sc_core::Simcontext::get().getGlobalTime() < sc_core::sc_time(50, sc_core::SC_MS));

    sit.claim_interrupt();

    //The pending interrupt register should be cleared after claim
    assert(dut.pending_interrupts[0] == 0 && dut.pending_interrupts[1] == 0);

    return 0;

    sit.was_triggered = false;

    uint32_t j = 2; //klee_int("j interrupt number");

    dut.gateway_trigger_interrupt(j);
    minikernel_step();
    if(j < 32)
        assert(dut.pending_interrupts[0] > 0);
    else
        assert(dut.pending_interrupts[1] > 0);
    assert(!sit.was_triggered);

	minikernel_step();
	minikernel_step(); //just for fun

	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	return 0;
}

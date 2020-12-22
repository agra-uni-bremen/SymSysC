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

void functional_test(PLIC<1, numberInterrupts, maxPriority>& dut, Simple_interrupt_target& sit)
{
	uint32_t i = klee_int("i interrupt number");

	klee_assume(i < numberInterrupts);

    dut.gateway_trigger_interrupt(i);

    //Is the pending interrupts register changed?
    if(i < 32)
        assert(dut.pending_interrupts[0] > 0);
    else
        assert(dut.pending_interrupts[1] > 0);

    auto before = sc_core::Simcontext::get().getGlobalTime();
    minikernel_step();	// should be 10ns
    // was PLIC fast enough?
    auto after = sc_core::Simcontext::get().getGlobalTime();
    assert(after-before == sc_core::sc_time(10, sc_core::SC_NS));

    //the step should trigger an external interrupt
    assert(sit.was_triggered);

    // Is correct Interrupt claimable?
    sit.claim_interrupt();

    //The pending interrupt register should be cleared after claim
    assert(dut.pending_interrupts[0] == 0 && dut.pending_interrupts[1] == 0);
}


void interface_test(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	unsigned constexpr max_data_length = 1000;
	unsigned data_length = klee_int("data_length");
	klee_assume(data_length <= max_data_length);
	uint8_t data[max_data_length];
    sc_core::sc_time delay;
    tlm::tlm_generic_payload pl;
    pl.set_read();
    pl.set_address(klee_int("address"));
    pl.set_data_length(data_length);
    pl.set_data_ptr(data);

    dut.transport(pl, delay);
}


int main(int argc, char* argv[])
{
	PLIC<1, numberInterrupts, maxPriority> dut("DUT");
    Simple_interrupt_target sit(dut);
	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;



	unsigned test = 0;
	if(argc == 2)
	{
		switch(argv[1][0])
		{
		//fall-through
		case '5': test++;
		case '4': test++;
		case '3': test++;
		case '2': test++;
		case '1': test++;
		case '0': break;
		default:
			INFO(std::cout << "Invalid testnumber given. Running all (0) benches" << std::endl);
		}
	} else
	{
		INFO(std::cout << "No testnumber given. Running all (0) benches" << std::endl);
		//Test info output
		sc_core::sc_time a(1002, sc_core::SC_MS), b (1, sc_core::SC_SEC), c (1, sc_core::SC_FS);
		INFO(std::cout << a.to_string() << " + " << b.to_string() << " = " << (a+b).to_string() << std::endl);
		INFO(std::cout << c.to_string() << " in default units is " << c.to_default_time_units() << " " << sc_core::unit_to_string(sc_core::default_time_unit) << std::endl);
		INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
		sc_core::Simcontext::get().printInfo();
	}


	minikernel_step();	//0ms

	if(test == 1 || test == 0)
		functional_test(dut, sit);
	if(test == 2 || test == 0)
		interface_test(dut);

	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	return 0;
}

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
        assert(!was_triggered && "interrupt triggered more than once");
        //TODO: Assert double triggered interrupt?
        // assert(!was_triggered && "double triggered interrupt");
        was_triggered = true;
        was_cleared = false;
    };

    void clear_external_interrupt()
    {
        INFO(std::cout << "Interrupt cleared" << std::endl);
        was_cleared = true;
    };

    uint32_t claim_interrupt()
    {
        assert(was_triggered && "tried to claim untriggered interrupt target");
        was_triggered = false;

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

		pl.set_write();
		dut.transport(pl, delay);	// notify finished interrupt

		//interrupt was either cleared or triggered for another prio
		assert(was_cleared || was_triggered);

		INFO(std::cout << "Interrupt " << interrupt << " claimed" << std::endl);
		return interrupt;
    }
};

void functional_test_basic(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	Simple_interrupt_target sit(dut);
	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;

	uint32_t i = klee_int("interrupt number");

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

    // was interrupt cleared by claiming?
    assert(sit.was_cleared && "Interrupt was not cleared after claim");

    //The pending interrupt register should be cleared after claim
    assert(dut.pending_interrupts[0] == 0 && dut.pending_interrupts[1] == 0);
}

void functional_test_priority_direct(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	Simple_interrupt_target sit(dut);
	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;

	uint32_t a = klee_int("a interrupt");
	uint32_t b = klee_int("b interrupt");
	INFO(a=2; b=1);

	uint32_t a_prio = klee_int("a interrupt priority");
	uint32_t b_prio = klee_int("j interrupt priority");
	INFO(a_prio=1; b_prio=2);


	klee_assume(a < numberInterrupts);
	klee_assume(b < numberInterrupts);
	//skip reserved interrupt 0
	klee_assume(a > 0);
	klee_assume(b > 0); //[1...31]
	klee_assume(a != b);

	klee_assume(a_prio < numberInterrupts); //[0...31]
	klee_assume(b_prio < numberInterrupts);

	//Direct write into member, skipping transport
	dut.interrupt_priorities[a] = a_prio;
	dut.interrupt_priorities[b] = b_prio;

	//first trigger may be low or high prio
	dut.gateway_trigger_interrupt(a);
	dut.gateway_trigger_interrupt(b);

	uint32_t first_itr  = a_prio > b_prio ? a : b;
	uint32_t second_itr = a_prio > b_prio ? b : a;
	if(a_prio == b_prio)
	{	// if same prio, itr num wins
		first_itr  = a < b ? a : b;
		second_itr = a < b ? b : a;
	}

	uint32_t actual_first_itr = dut.hart_get_next_pending_interrupt(0, false);

	assert(actual_first_itr == first_itr &&
			"Invalid interrupt priority calculated");

	dut.clear_pending_interrupt(first_itr);

	uint32_t actual_second_itr = dut.hart_get_next_pending_interrupt(0, false);

	assert(actual_second_itr == second_itr &&
			"Invalid interrupt priority calculated");
}

void functional_test_consider_threshold(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	Simple_interrupt_target sit(dut);
	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;

	uint32_t a = klee_int("a interrupt");
	INFO(a=2);

	klee_assume(a < numberInterrupts);
	//skip reserved interrupt 0
	klee_assume(a > 0);

	uint32_t a_prio = klee_int("a interrupt priority");
	INFO(a_prio=1);
	klee_assume(a_prio < numberInterrupts); //[0...31]

	uint32_t hart_consider_thr = klee_int("hart consider threshold");
	klee_assume(hart_consider_thr < numberInterrupts);



	//Direct write into member, skipping transport
	dut.interrupt_priorities[a] = a_prio;
	dut.hart_priority_threshold[0] = hart_consider_thr;

	//first trigger may be low or high prio
	dut.gateway_trigger_interrupt(a);

	uint32_t itr = dut.hart_get_next_pending_interrupt(0, false);

	if(a_prio > hart_consider_thr)
		assert(itr == a && "Interrupt not considered");
	else
		assert(itr == 0 && "Interrupt was considered");
}

void functional_test_itr_num_priority(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	Simple_interrupt_target sit(dut);
	//interrupt line plic -> sit
	dut.target_harts[0] = &sit;


	uint32_t a = klee_int("a interrupt");
	uint32_t b = klee_int("b interrupt");
	INFO(b=2);

	klee_assume(a < numberInterrupts);
	klee_assume(b < numberInterrupts);
	//skip reserved interrupt 0
	klee_assume(a > 0);
	klee_assume(b > 0);
	klee_assume(a != b);

	uint32_t lower_itr = a < b ? a : b;
	uint32_t highr_itr = a > b ? a : b;

	//first trigger may be low or high prio
	dut.gateway_trigger_interrupt(a);
	dut.gateway_trigger_interrupt(b);

    minikernel_step();

    //the step should trigger an external interrupt
    assert(sit.was_triggered &&
    		"No triggered interrupt");

    // Is correct Interrupt claimable?
    uint32_t first_itr = sit.claim_interrupt();

    //Was the itr with the highest prio (lowest val) chosen first?
    assert(first_itr == lower_itr &&
    		"Invalid interrupt sequence");

    minikernel_step();

    assert(sit.was_triggered &&
			"interrupt was not triggered again for second itr");

    // Is correct Interrupt claimable?
    uint32_t second_itr = sit.claim_interrupt();

    //Was the itr with the highest prio (lowest val) chosen?
    assert(second_itr == highr_itr &&
    		"Second itr not from lower prio");

    assert(sit.was_cleared &&
    		"Interrupt was not cleared after claim of last itr");
}

void interface_test_read(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	dut.gateway_trigger_interrupt(1);

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

	minikernel_step();
	minikernel_step();
}

void interface_test_write(PLIC<1, numberInterrupts, maxPriority>& dut)
{
	dut.gateway_trigger_interrupt(1);

	unsigned constexpr max_data_length = 100;
	unsigned data_length = klee_int("data_length");
	klee_assume(data_length <= max_data_length);
	uint8_t data[max_data_length];
	klee_make_symbolic(data, max_data_length, "write data");
	sc_core::sc_time delay;
	tlm::tlm_generic_payload pl;
	pl.set_write();
	pl.set_address(klee_int("address"));
	pl.set_data_length(data_length);
	pl.set_data_ptr(data);

	dut.transport(pl, delay);

	minikernel_step();
	minikernel_step();
}


int main(int argc, char* argv[])
{
	PLIC<1, numberInterrupts, maxPriority> dut("DUT");
	minikernel_step();	//0ms

	if(argc == 2)
	{
		if(strcmp(argv[1], "functional_test_basic") == 0)
			functional_test_basic(dut);

		else if(strcmp(argv[1], "functional_test_itr_num_priority") == 0)
			functional_test_itr_num_priority(dut);

		else if(strcmp(argv[1], "functional_test_consider_threshold") == 0)
			functional_test_consider_threshold(dut);

		else if(strcmp(argv[1], "functional_test_priority_direct") == 0)
			functional_test_priority_direct(dut);

		else if(strcmp(argv[1], "interface_test_read") == 0)
			interface_test_read(dut);

		else if(strcmp(argv[1], "interface_test_write") == 0)
			interface_test_write(dut);

		else
			INFO(std::cout << "Invalid test given." << std::endl);

	} else
	{
		INFO(std::cout << "No test given." << std::endl);
		//Test info output
		sc_core::sc_time a(1002, sc_core::SC_MS), b (1, sc_core::SC_SEC), c (1, sc_core::SC_FS);
		INFO(std::cout << a.to_string() << " + " << b.to_string() << " = " << (a+b).to_string() << std::endl);
		INFO(std::cout << c.to_string() << " in default units is " << c.to_default_time_units() << " " << sc_core::unit_to_string(sc_core::default_time_unit) << std::endl);
		INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
		sc_core::Simcontext::get().printInfo();
	}


	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	return 0;
}

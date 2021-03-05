#pragma once

#include <tlm_utils/simple_target_socket.h>
#include <systemc>
#include "core/common/irq_if.h"
#include "util/tlm_map.h"
#include <array>
#include <stdint.h>

template <unsigned NumberCores, unsigned NumberInterrupts, uint32_t MaxPriority>
struct PLIC : public sc_core::sc_module, public interrupt_gateway {
	static_assert(NumberInterrupts <= 4096, "out of bound");
	static_assert(NumberCores <= 15360, "out of bound");

	tlm_utils::simple_target_socket<PLIC> tsock;

	std::array<external_interrupt_target*, NumberCores> target_harts{};

	// shared for all harts
	// priority 1 is the lowest. Zero means do not interrupt
	// NOTE: addressing starts at 1 because interrupt 0 is reserved
	std::array<uint32_t, NumberInterrupts> interrupt_priorities{};

	// how many 32bit entries are required to hold all interrupts
	static constexpr unsigned NumberInterruptEntries = NumberInterrupts + NumberInterrupts%32;

	std::array<uint32_t, NumberInterruptEntries> pending_interrupts{};

	std::array<std::array<uint32_t, NumberInterruptEntries>, NumberCores> hart_enabled_interrupts{};
	std::array<uint32_t, NumberCores> hart_priority_threshold{};
	std::array<uint32_t, NumberCores> hart_claim_response{};
	std::array<bool, NumberCores> hart_eip{};

	vp::map::LocalRouter router = {"PLIC"};

	sc_core::sc_event e_run;
	sc_core::sc_time clock_cycle;

	SC_HAS_PROCESS(PLIC);
	// macro-replaced to
	//typedef PLIC SC_CURRENT_USER_MODULE;

	void run();

	PLIC(sc_core::sc_module_name nem) : sc_module(nem){
		clock_cycle = sc_core::sc_time(10, sc_core::SC_NS);
		tsock.register_b_transport(this, &PLIC::transport);

		auto &regs = router
				.add_register_bank({})
				.register_handler(this, &PLIC::register_access_callback);

		for (unsigned i=0; i<NumberInterrupts; ++i) {
			interrupt_priorities[i] = 1;
			regs.add_register({0x0 + i*4, &interrupt_priorities[i]});
		}

		for (unsigned i=0; i<NumberInterruptEntries; ++i) {
			regs.add_register({0x1000 + i*4, &pending_interrupts[i], vp::map::read_only});
		}

		for (unsigned n=0; n<NumberCores; ++n) {
			for (unsigned i=0; i<NumberInterruptEntries; ++i) {
				hart_enabled_interrupts[n][i] = 0xffffffff;	// all interrupts enabled by default
				regs.add_register({0x2000 + i*4 + n*NumberInterruptEntries*4, &hart_enabled_interrupts[n][i]});
			}

			regs.add_register({0x200000 + n*8, &hart_priority_threshold[n]});
			regs.add_register({0x200004 + n*8, &hart_claim_response[n]});
		}

		SC_THREAD(run);
		//Macro-replaced to
		//threads.push_back(std::bind(&SC_CURRENT_USER_MODULE::run, this));
	}


	void gateway_trigger_interrupt(uint32_t irq_id) {
		// NOTE: can use different techniques for each gateway, in this case a
		// simple non queued edge trigger

#ifdef EXTRA_BUGS
		assert(irq_id > 0 && irq_id <= NumberInterrupts);
#else
		assert(irq_id > 0 && irq_id < NumberInterrupts);
#endif
		// std::cout << "[vp::plic] incoming interrupt " << irq_id << std::endl;

		unsigned idx = irq_id / 32;
		unsigned off = irq_id % 32;

		pending_interrupts[idx] |= 1 << off;

#ifdef EXTRA_BUGS
		if(irq_id == 13)	//irq 13 not triggered
					return;

		if(irq_id > 32) {
			//irq > 32 is slower than specified
			e_run.notify(clock_cycle+sc_core::sc_time(5, sc_core::SC_NS));
			return;
		}
#endif

		e_run.notify(clock_cycle);
	}


	void clear_pending_interrupt(unsigned irq_id) {
		assert(irq_id > 0 && irq_id < NumberInterrupts);
		INFO(std::cout << "[vp::plic] clear pending interrupt " << irq_id << std::endl);

#ifdef EXTRA_BUGS
		if(irq_id == 16)
			return;
#endif

		unsigned idx = irq_id / 32;
		unsigned off = irq_id % 32;

		pending_interrupts[idx] &= ~(1 << off);
	}


	unsigned hart_get_next_pending_interrupt(unsigned hart_id, bool consider_threshold) {
		unsigned min_id = 0;
		unsigned max_priority = 0;

		for (unsigned i=1; i<NumberInterrupts; ++i) {
			unsigned idx = i / 32;
			unsigned off = i % 32;

			if (hart_enabled_interrupts[hart_id][idx] & (1 << off)) {
				if (pending_interrupts[idx] & (1 << off)) {
					auto prio = interrupt_priorities[i];
#ifdef EXTRA_BUGS
					if (prio > 0 && (!consider_threshold || (prio >= hart_priority_threshold[hart_id]))) {
#else
					if (prio > 0 && (!consider_threshold || (prio > hart_priority_threshold[hart_id]))) {
#endif
						if (prio > max_priority) {
							max_priority = prio;
							min_id = i;
						}
					}
				}
			}
		}

		return min_id;
	}


	void register_access_callback(const vp::map::register_access_t &r) {
		if (r.write && (r.addr >= 0x1000 && r.addr < 0x1000+NumberInterruptEntries*4)) {
			//NON-Intentional BUG: Checks only for addr, not length
			assert(false && "pending interrupts registers are read only");
			return;
		}

		if (r.read) {
			for (unsigned i=0; i<NumberCores; ++i) {
				if (r.vptr == &hart_claim_response[i]) {
					// NOTE: on claim request retrieve return and clear the interrupt with
					// highest priority, priority threshold is ignored at this point
					unsigned min_id = hart_get_next_pending_interrupt(0, false);
					hart_claim_response[i] = min_id;  // zero means no more interrupt to claim
					clear_pending_interrupt(min_id);
					INFO(std::cout << "[vp::plic] claim interrupt " << min_id << std::endl);
					break;
				}
			}
		}

		r.fn();

		if (r.write) {
			for (unsigned i=0; i<NumberCores; ++i) {
				if (r.vptr == &hart_claim_response[i]) {
					// NOTE: on completed response, check if there are any other pending
					// interrupts
					if (hart_has_pending_enabled_interrupts(i)) {
						assert(hart_eip[i]);	//BUG: HERE Overflow
						// trigger again to make this work even if the SW clears the harts interrupt pending bit
#ifndef EXTRA_BUGS
						target_harts[i]->trigger_external_interrupt();
#endif
					} else {
						hart_eip[i] = false;
						target_harts[i]->clear_external_interrupt();
					}
					// std::cout << "[vp::plic] clear eip" << std::endl;
					break;
				}
			}

			// ensure the priority value is valid
			if (r.addr <= 0x0FFF) {
				for (auto &x : interrupt_priorities)
					x = std::min(x, MaxPriority);
			}

			// interrupt priority, hart priority threshold or enabled interrupts may have changed, thus trigger re-checking of pending interrupts
			e_run.notify(10*clock_cycle);
		}
	}


	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		router.transport(trans, delay);
	}


	bool hart_has_pending_enabled_interrupts(unsigned hart_id) {
		return hart_get_next_pending_interrupt(hart_id, true) > 0;
	}
};

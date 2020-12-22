#ifndef RISCV_ISA_SENSOR_H
#define RISCV_ISA_SENSOR_H

#include <cstdlib>
#include <cstring>
#include <stddef.h>
#include <systemc>

#include <tlm_utils/simple_target_socket.h>

#include "core/common/irq_if.h"

struct SimpleSensor : public sc_core::sc_module {
	tlm_utils::simple_target_socket<SimpleSensor> tsock;

	interrupt_gateway *plic = 0;
	uint32_t irq_number = 0;
	sc_core::sc_event run_event;

	union Memory {
		struct {
			uint8_t  data_frame[64];
			uint32_t scaler;
			uint32_t filter;
		};
		uint8_t raw[64 + sizeof(uint32_t) + sizeof(uint32_t)];
	} memory;


	SC_HAS_PROCESS(SimpleSensor);

	SimpleSensor(uint32_t irq_number) : irq_number(irq_number) {
		memory.scaler = 40;
		memory.filter =  0;
		tsock.register_b_transport(this, &SimpleSensor::transport);
		SC_THREAD(run);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		auto addr = trans.get_address();
		auto cmd = trans.get_command();
		auto len = trans.get_data_length();
		auto ptr = trans.get_data_ptr();

		assert(len > 0 && "invalid read with length = 0");

		if (addr <= 63) {
			// access data frame
			assert(cmd == tlm::TLM_READ_COMMAND && "data is read only");
			assert((addr + len) <= sizeof(memory.data_frame) && "read out of memory map");

			// return last generated random data at requested address
			memcpy(ptr, &memory.data_frame[addr], len);
		} else {
			assert(len == 4 && "only allow to read/write whole register");

			//intentional bug: not taking + len into account
			assert(addr <= offsetof(union Memory, filter) && "access to non-mapped address");

			// trigger pre read/write actions
			if (cmd == tlm::TLM_WRITE_COMMAND) {
				if(addr == offsetof(union Memory, scaler)) {
					uint32_t value = *((uint32_t *)ptr);
					if (value < 1 || value > 100)
						return;  // ignore invalid values

					run_event.cancel();
					run_event.notify(sc_core::sc_time(value, sc_core::SC_MS));
				}
				memcpy(&memory.raw[addr], ptr, len);
			}
			else if (cmd == tlm::TLM_READ_COMMAND) {
				memcpy(ptr, &memory.raw[addr], len);
			} else {
				assert(false && "unsupported tlm command for sensor access");
			}

		}

		(void)delay;  // zero delay
	}

	void run() {

		// header--
		enum class Label
		{
			init,
			here1
		};

		static Label position = Label::init;

		switch (position)
		{
			case Label::here1:
				goto HERE1;
			default:
				//nothing
				break;
		}
		//INFO(std::cout << "run init" << std::endl);
		// --header

		while (true) {
			run_event.notify(sc_core::sc_time(memory.scaler, sc_core::SC_MS));

			//INFO(std::cout << "run wait()" << std::endl);
			sc_core::wait(run_event);  // 40 times per second by default
			position = Label::here1;
			return;
HERE1:

			// fill with random data
			for (auto &n : memory.data_frame) {
				if (memory.filter == 1) {
					n = rand() % 10 + 48;
				} else if (memory.filter == 2) {
					n = rand() % 26 + 65;
				} else {
					// fallback for all other filter values
					n = rand() % 92 + 32;  // random printable char
				}
			}

			// intentional bug: Interrupt 64 will not be fired
			if(irq_number != 64)
				plic->gateway_trigger_interrupt(irq_number);
		}
	}
};

#endif  // RISCV_ISA_SENSOR_H

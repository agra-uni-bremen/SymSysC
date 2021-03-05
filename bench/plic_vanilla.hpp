#include "plic.hpp"

template <unsigned NumberCores, unsigned NumberInterrupts, uint32_t MaxPriority>
void PLIC<NumberCores,NumberInterrupts,MaxPriority>::run() {
	while (true) {
		sc_core::wait(e_run);
		for (unsigned i=0; i<NumberCores; ++i) {
			if (!hart_eip[i]) {
				if (hart_has_pending_enabled_interrupts(i)) {
					// std::cout << "[vp::plic] trigger interrupt" << std::endl;
					hart_eip[i] = true;
					target_harts[i]->trigger_external_interrupt();
				}
			}
		}
	}
}

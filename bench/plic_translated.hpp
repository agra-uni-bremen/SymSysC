#include "plic.hpp"

template <unsigned NumberCores, unsigned NumberInterrupts, uint32_t MaxPriority>
void PLIC<NumberCores,NumberInterrupts,MaxPriority>::run() {

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
	INFO(std::cout << "run init" << std::endl);
	// --header

	while (true) {
		INFO(std::cout << "run wait()" << std::endl);
		sc_core::wait(e_run);
		position = Label::here1;
		return;
HERE1:

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

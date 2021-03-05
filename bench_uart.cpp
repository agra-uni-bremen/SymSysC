#include "bench/uart16550.h"
#include "minikernel/registry.hpp"
#include "minikernel/klee_conf.h"


int main()
{
	UART16550 dut("bernd");

	INFO(std::cout << "Number of registered transports: " << transports.size() << std::endl);
	sc_core::Simcontext::get().printInfo();


	sc_core::sc_time delay;
    tlm::tlm_generic_payload pl;
    uint32_t address = klee_int("address");
    uint32_t length = klee_int("length");
    const uint32_t max_len = 1000;

    klee_assume(length <= max_len);
    pl.set_read();
    pl.set_address(address);
    pl.set_data_length(length);
    uint8_t* buffer = new uint8_t[max_len];
    pl.set_data_ptr(buffer);

    dut.transport(pl, delay);

	INFO(std::cout << "finished at " << minikernel_current_time() << std::endl);

	delete[] buffer;
	return 0;
}

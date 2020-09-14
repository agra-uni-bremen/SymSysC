#include <iostream>

#include "bench/plic.h"
#include "sim/systemc.h"


int main()
{

	PLIC<1, 32, 32> dut("DUT");

	std::cout << "Number of registered transports: " << transports.size() << std::endl;
	std::cout << "              ""      processes: " << processes.size() << std::endl;
	std::cout << "              ""        threads: " << threads.size() << std::endl;

	std::cout << "lol" << std::endl;

	return 0;
}

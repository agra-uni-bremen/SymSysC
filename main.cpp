#include <iostream>

#include "bench/plic.h"
#include "sim/systemc.h"


int main()
{

	PLIC<1, 32, 32> dut("DUT");

	std::cout << "lol" << std::endl;

	return 0;
}

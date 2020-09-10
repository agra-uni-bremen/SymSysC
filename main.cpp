#include <iostream>

#include "sym/wrap.h"
#include "bench/plic.h"


int main()
{

	PLIC<1, 32, 32> dut("DUT");

	std::cout << "lol" << std::endl;

	return 0;
}

#include "registry.hpp"
//#include <iostream>
//here is the actual place

std::vector<std::function<Transportfunction>> transports;
std::vector<std::function<Thread>> threads;
std::vector<std::function<Process>> processes;


void run_all_threads()
{
	//std::cout << "running all threads" << std::endl;
	for(auto &t : threads)
	{
		t();
	}
}

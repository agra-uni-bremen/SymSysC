#pragma once

#include "sc_time.h"
#include "iostream"

namespace sc_core
{

class sc_event
{
	//dunno, something pollable? Maybe also an sc_event registry?

public:
	//notify for use in benchmark to require a certain reaction
	//void notify(const sc_time& event) { std::cout << "Event at " << event.m_time << std::endl; };
	void notify(const sc_time event) { std::cout << "Event at " << event.m_time << std::endl; };
};
}

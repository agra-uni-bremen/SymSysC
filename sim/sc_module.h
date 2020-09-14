#pragma once
#include "time.h"
#include "../sim/sc_module_name.h"

namespace sc_core{

class sc_module
{
	sc_module_name m_name;
protected:
	sc_time clock_cycle = sc_time(10, sc_core::SC_NS);
public:
	sc_module(sc_module_name name) : m_name(name){};
};

}

#pragma once
#include "time.h"
#include "../sim/sc_module_name.h"

//


#define SC_HAS_PROCESS( user_module_name )                                      \
    typedef user_module_name SC_CURRENT_USER_MODULE

#define SC_THREAD( fun )														\
	threads.push_back(std::bind(&SC_CURRENT_USER_MODULE::fun, this))

#define SC_PROCESS( fun )														\
	processes.push_back(std::bind(&SC_CURRENT_USER_MODULE::fun, this))


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

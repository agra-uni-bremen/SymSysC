#pragma once
#include "sc_time.h"
#include "sc_module_name.h"
#include "sc_simcontext.h"

//


#define SC_HAS_PROCESS( user_module_name )                                      \
    typedef user_module_name SC_CURRENT_USER_MODULE

#define SC_THREAD( fun )														\
	sc_core::Simcontext::get().addThread(std::bind(&SC_CURRENT_USER_MODULE::fun, this))

#define SC_PROCESS( fun )														\
	sc_core::Simcontext::get().addProcess(std::bind(&SC_CURRENT_USER_MODULE::fun, this))


namespace sc_core{

class sc_module
{
	sc_module_name m_name;
protected:

public:
	sc_module(sc_module_name name) : m_name(name){};
};

}

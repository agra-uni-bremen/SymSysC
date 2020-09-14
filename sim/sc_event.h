#pragma once

#include "sc_time.h"


namespace sc_core
{

class sc_event
{
	//dunno, something pollable? Maybe also an sc_event registry?

public:
	void notify(sc_time& event) {};
};
}

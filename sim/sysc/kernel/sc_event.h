#pragma once

#include "sc_time.h"

namespace sc_core
{

struct sc_event
{
	//dunno, something pollable? Maybe also an sc_event registry?
    sc_time time_to_wake;

public:
	//notify for use in benchmark to require a certain reaction
    void notify(const sc_time& time);
};


void wait(const sc_event& event);

}

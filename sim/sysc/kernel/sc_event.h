#pragma once

#include "sc_time.h"
#include "sc_simcontext.h"
#include <functional>

namespace sc_core
{

/*
 * Is it allowed for an Event to be notified before anyone waits for it?
 *
 */


struct sc_event
{
    std::function<Thread>* waitingThread = nullptr;

public:
	//notify for use in benchmark to require a certain reaction
    void notify(const sc_time& time);
    //sc_time getWaketime();
};


void wait(sc_event& event);
void wait(const sc_time& time);

}

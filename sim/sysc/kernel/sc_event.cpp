#include "sc_event.h"
#include "sc_simcontext.h"
#include <klee_conf.h>

namespace sc_core
{

void sc_event::notify(const sc_time& time) {
    INFO(std::cout << "notify in " << time.to_string() << std::endl);

    assert(waitingThread != nullptr && "Notify on unwaited event");
    Simcontext::get().addWaketime(waitingThread, Simcontext::get().getGlobalTime() + time);
};

/*
sc_time sc_event::getWaketime()
{
    return time_to_wake;
}
*/

void wait(sc_event& event)
{
    event.waitingThread = Simcontext::getActiveThread();
}

void wait(const sc_time& time)
{
    Simcontext::get().addWaketime(Simcontext::get().getGlobalTime() + time);
}
}

#include "sc_event.h"
#include "sc_simcontext.h"
#include <klee_conf.h>

namespace sc_core
{

void sc_event::notify(const sc_time& time) {
    INFO(std::cout << "notify in " << time.to_string() << std::endl);

    //assert(waitingThread != nullptr && "Notify on unwaited event");
    auto notified_thread = waitingThread;
    if(notified_thread == nullptr)	//this may happen if a notify is inside thread
    	notified_thread = Simcontext::getActiveThread();
    Simcontext::get().addWaketime(notified_thread, Simcontext::get().getGlobalTime() + time);
};

void sc_event::cancel() {
    INFO(std::cout << "event cancel" << std::endl);

    assert(waitingThread != nullptr && "cancel on unwaited event");
    Simcontext::get().removeWaketime(waitingThread);
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

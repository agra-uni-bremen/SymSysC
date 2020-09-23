#include "sc_event.h"
#include <klee_conf.h>

namespace sc_core
{

sc_time global_time;

void sc_event::notify(const sc_time& time) {
    INFO(std::cout << "notify in " << time.to_string() << std::endl);
    time_to_wake = global_time + time;
};

void wait(const sc_event& event)
{
    INFO(std::cout << "NOT waiting for event at " << event.time_to_wake.to_string() << std::endl);
}

}

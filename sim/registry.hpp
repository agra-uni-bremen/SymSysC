#pragma once

#include <functional>
#include <vector>
#include <tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h>
#include <sysc/kernel/sc_time.h>

typedef void Transportfunction(tlm::tlm_generic_payload& , sc_core::sc_time&);
typedef void Thread(void);
typedef void Process(void);

extern std::vector<std::function<Transportfunction>> transports;
extern std::vector<std::function<Thread>> threads;
extern std::vector<std::function<Process>> processes;

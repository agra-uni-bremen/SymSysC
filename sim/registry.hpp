#pragma once


#include "sysc/kernel/sc_simcontext.h"

typedef void Transportfunction(tlm::tlm_generic_payload& , sc_core::sc_time&);

extern std::vector<std::function<Transportfunction>> transports;

void run_all_threads();

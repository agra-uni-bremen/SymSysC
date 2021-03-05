#pragma once


#include "sysc/kernel/sc_simcontext.h"

typedef void Transportfunction(tlm::tlm_generic_payload& , sc_core::sc_time&);

extern std::vector<std::function<Transportfunction>> transports;

void minikernel_step();
std::string minikernel_current_time();

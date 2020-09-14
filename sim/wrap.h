//#include <tlm_utils/simple_target_socket.h>
#pragma once
#include <functional>
#include <vector>

#include "sc_module.h"
#include "sc_time.h"
#include "tlm_gp.h"



#define SC_HAS_PROCESS( shit ) ;




#define SC_THREAD( fun ) threads.push_back(std::bind(this, fun))

std::vector<std::function<Transportfunction>> transports;
std::vector<std::function<void(void)>> threads;
std::vector<std::function<void(void)>> processes;

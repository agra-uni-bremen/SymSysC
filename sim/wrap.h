//#include <tlm_utils/simple_target_socket.h>
#pragma once
#include <functional>
#include <vector>

#include "sc_module.h"
#include "sc_time.h"
#include "tlm_gp.h"


typedef void Thread(void);
typedef void Process(void);

std::vector<std::function<Transportfunction>> transports;
std::vector<std::function<Thread>> threads;
std::vector<std::function<Process>> processes;

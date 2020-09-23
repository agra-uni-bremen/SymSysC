/*
 * sc_simcontext.h
 *
 *  Created on: Sep 23, 2020
 *      Author: dwd
 */

#pragma once

#include <functional>
#include <vector>
#include <tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h>
#include <sysc/kernel/sc_time.h>
#include <klee_conf.h>

namespace sc_core
{


typedef void Thread(void);
typedef void Process(void);


class Simcontext
{

    std::vector<std::function<Thread>> threads;
    std::vector<std::function<Process>> processes;

    sc_time global_time;

public:

    static Simcontext& get() {
        static Simcontext context;
        return context;
    }

    void addThread(std::function<Thread> t) {
        threads.push_back(t);
    }

    void addProcess(std::function<Process> t) {
        processes.push_back(t);
    }

    void runNextStep()
    {
        INFO(std::cout << "runNextStep not yet implemented" << std::endl);
    }

    void printInfo()
    {
        INFO(std::cout << "                      processes: " << processes.size() << std::endl);
        INFO(std::cout << "                        threads: " << threads.size() << std::endl);
    }

private:
    Simcontext(){};
};




};



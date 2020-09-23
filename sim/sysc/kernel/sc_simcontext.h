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
    //TODO: Prio-queue got Threads
    typedef std::pair<std::function<Thread>,sc_time> Threadlist;
    std::vector<Threadlist> threads;
    std::vector<std::function<Process>> processes;

    sc_time global_time;

    std::function<Process>* activeThread = nullptr;


public:

    static Simcontext& get() {
        static Simcontext context;
        return context;
    }

    sc_time getGlobalTime()
    {
        return global_time;
    }

    void addThread(std::function<Thread> t) {
        threads.push_back(Threadlist(t, sc_time(0, default_time_unit)));
    }

    void addProcess(std::function<Process> p) {
        processes.push_back(p);
    }

    void runNextStep()
    {
        for(Threadlist& p : threads)
        {
            if(p.second == global_time)
            {
                activeThread = &p.first;
                p.first();
            }
        }
        activeThread = nullptr;

        //TODO: step the biggest possible time forward
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



/*
 * sc_simcontext.h
 *
 *  Created on: Sep 23, 2020
 *      Author: dwd
 */

#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <assert.h>
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
    typedef std::pair<sc_time, std::function<Thread>*> Wakelist;
    std::vector<std::function<Thread>> threads;
    std::vector<std::function<Process>> processes;
    std::priority_queue<Wakelist, std::vector<Wakelist>, std::greater<Wakelist>> wakelist;

    sc_time global_time;

    std::function<Process>* activeThread = nullptr;


public:

    static Simcontext& get() {
        static Simcontext context;
        return context;
    }

    static std::function<Process>* getActiveThread()
    {
        return get().activeThread;
    }

    sc_time getGlobalTime()
    {
        return global_time;
    }

    void addThread(std::function<Thread> t) {
        threads.push_back(t);
        wakelist.push(Wakelist{sc_time(0, SC_MS), &threads[threads.size()-1]});
    }

    void addProcess(std::function<Process> p) {
        processes.push_back(p);
    }

    void addWaketime(std::function<Thread>* thread, const sc_core::sc_time& time)
    {
        assert(thread != nullptr && "add Waketime with invalid active Thread!");
        wakelist.push(Wakelist{time, thread});
    }

    void addWaketime(const sc_core::sc_time& time)
    {
        addWaketime(activeThread, time);
    }

    void runNextStep()
    {
        if(wakelist.empty())
        {
            INFO(std::cout << "No waiting threads" << std::endl);
            return;
        }

        assert(wakelist.top().first >= global_time && "Next wake event lies in the past!");
        global_time = wakelist.top().first;

        INFO(std::cout << "Running threads at " << global_time.to_string() << std::endl);

        while(!wakelist.empty() && wakelist.top().first == global_time)
        {
            activeThread = wakelist.top().second;
            wakelist.pop();
            (*activeThread)();

        }
        activeThread = nullptr;
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



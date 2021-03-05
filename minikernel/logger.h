/*
 * logger.h
 *
 *  Created on: Sep 23, 2020
 *      Author: dwd
 */
#pragma once

#include <iostream>

class Logger
{
    bool enabled;
public:
    void disable();
    void enable();

    static Logger& log() {
        static Logger instance;
        return instance;
    }

    template<typename T>
    friend Logger& operator<<(Logger& log, const T& val);
private:
    Logger(): enabled(true){};
};


template<typename T>
Logger& operator << (Logger& log, const T& val) {
    if(log.enabled)
        std::cout << val;
    return log;
}


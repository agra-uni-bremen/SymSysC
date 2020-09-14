/*
 * sc_time.h
 *
 *  Created on: Sep 10, 2020
 *      Author: dwd
 */

#pragma once
#include <inttypes.h>

namespace sc_core
{
	enum sc_time_unit
	{
		SC_FS = 0,
		SC_PS,
		SC_NS,
		SC_US,
		SC_MS,
		SC_SEC
	};

	struct sc_time
	{
		uint64_t m_time;
		sc_time_unit m_unit;
		sc_time(uint64_t time, sc_time_unit unit) : m_time(time),m_unit(unit){};
	};
};

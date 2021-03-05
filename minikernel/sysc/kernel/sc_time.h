/*
 * sc_time.h
 *
 *  Created on: Sep 10, 2020
 *      Author: dwd
 */

#pragma once
#include <inttypes.h>
#include <string>

//see sysc/kernel/sc_time.h


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

	extern sc_time_unit default_time_unit;

	std::string unit_to_string(const sc_time_unit& unit);
	uint64_t to_factor(const sc_time_unit unit);
	double factor_diff(const sc_time_unit from, const sc_time_unit to);
	uint64_t abs_factor_diff(const sc_time_unit a, const sc_time_unit b);
    inline sc_time_unit biggest_unit(const sc_time_unit a, const sc_time_unit b);
    inline sc_time_unit smallest_unit(const sc_time_unit a, const sc_time_unit b);

	struct sc_time
	{
		uint64_t m_time;
		sc_time_unit m_unit;
		sc_time() : m_time(0),m_unit(default_time_unit){};
		sc_time(uint64_t time, sc_time_unit unit) : m_time(time),m_unit(unit){};

	    // assignment operator

	    //sc_time& operator = ( const sc_time& );


	    // conversion functions

	    uint64_t value() const;      // relative to the time resolution
	    double to_double() const;  // relative to the time resolution
	    double to_default_time_units() const;
	    uint64_t to_smaller_unit(const sc_time_unit other) const;
	    double to_seconds() const;
	    const std::string to_string() const;


	    // relational operators

	    bool operator == ( const sc_time& ) const;
	    bool operator != ( const sc_time& ) const;
	    bool operator <  ( const sc_time& ) const;
	    bool operator <= ( const sc_time& ) const;
	    bool operator >  ( const sc_time& ) const;
	    bool operator >= ( const sc_time& ) const;


	    // arithmetic operators

	    sc_time& operator += ( const sc_time& );
	    sc_time& operator -= ( const sc_time& );

	    friend const sc_time operator + ( const sc_time&, const sc_time& );
	    friend const sc_time operator - ( const sc_time&, const sc_time& );

	    sc_time& operator *= ( double );
	    sc_time& operator /= ( double );
	    sc_time& operator %= ( const sc_time& );

	    friend const sc_time operator * ( const sc_time&, double );
	    friend const sc_time operator * ( double, const sc_time& );
	    friend const sc_time operator / ( const sc_time&, double );
	    friend double        operator / ( const sc_time&, const sc_time& );
	    friend const sc_time operator % ( const sc_time&, const sc_time& );
	};


};

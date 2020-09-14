#include "sc_time.h"


namespace sc_core
{
	    sc_time& sc_time::operator = ( const sc_time& other)
	    {
	    	//better would by copy-copnstructor
	    	m_time = other.m_time;
	    	m_unit = other.m_unit;
	    	return *this;
	    }


	    // conversion functions

	    uint64_t sc_time::value() const
	    {
	    	return m_time;
	    }
	    double sc_time::to_double() const
	    {
	    	return m_time;
	    }

	    //double to_default_time_units() const;
	    //double to_seconds() const;
	    const std::string sc_time::to_string() const
	    {
	    	std::string msg = std::to_string(m_time);
	    	switch (m_unit)
	    	{
	    	case SC_FS:
	    		msg += " femtoseconds";
	    		break;
	    	case SC_PS:
	    		msg += " picoseconds";
	    		break;
	    	case SC_NS:
	    		msg += " nanoseconds";
	    		break;
	    	case SC_US:
	    		msg += " microseconds";
	    		break;
	    	case SC_MS:
	    		msg += " milliseconds";
	    		break;
	    	case SC_SEC:
	    		msg += " seconds";
	    		break;
	    	default:
	    		msg += " ERROR SECONDS";
	    	}
	    	return msg;
	    }



	    // relational operators

	    bool sc_time::operator == ( const sc_time& other) const
		{
	    	return m_unit == other.m_unit && m_time == other.m_time;
		}

	    /* TODO DO
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

		*/


	    const sc_time operator * ( const sc_time& left , double right)
		{
	    	return sc_time(left.m_time * right, left.m_unit);
		}
	    const sc_time operator * ( double left, const sc_time& right)
		{
	    	return sc_time(right.m_time * left, right.m_unit);
		}

	    /*
	    friend const sc_time operator / ( const sc_time&, double );
	    friend double        operator / ( const sc_time&, const sc_time& );
	    friend const sc_time operator % ( const sc_time&, const sc_time& );
	    */
};

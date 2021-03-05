#include "sc_time.h"


namespace sc_core
{

    sc_time_unit default_time_unit = SC_US;

    std::string unit_to_string(const sc_time_unit& unit)
    {
        switch (unit)
        {
        case SC_FS:
            return "femtoseconds";
        case SC_PS:
            return "picoseconds";
        case SC_NS:
            return "nanoseconds";
        case SC_US:
            return "microseconds";
        case SC_MS:
            return "milliseconds";
        case SC_SEC:
            return "seconds";
            break;
        default:
            return "ERROR SECONDS";
        }
    }

    uint64_t to_factor(const sc_time_unit unit)
    {
        uint64_t factor = 1;
        switch (unit)
        {
        case SC_FS:
            factor *= 1000;
            //fall-through
        case SC_PS:
            factor *= 1000;
            //fall-through
        case SC_NS:
            factor *= 1000;
            //fall-through
        case SC_US:
            factor *= 1000;
            //fall-through
        case SC_MS:
            factor *= 1000;
            //fall-through
        case SC_SEC:
            break;
        }
        return factor;
    }


    double factor_diff(const sc_time_unit from, const sc_time_unit to)
    {
        double factor_from = to_factor(from);
        double factor_to = to_factor(to);
        return factor_to / factor_from;
    }

    //same as factor_diff, but always >= 1
    uint64_t abs_factor_diff(const sc_time_unit a, const sc_time_unit b)
    {
        return std::max(to_factor(a), to_factor(b)) / std::min(to_factor(a), to_factor(b));
    }

    inline sc_time_unit biggest_unit(const sc_time_unit a, const sc_time_unit b)
    {
        return a > b ? a : b;
    }
    inline sc_time_unit smallest_unit(const sc_time_unit a, const sc_time_unit b)
    {
        return a < b ? a : b;
    }

    /*
    sc_time& sc_time::operator = ( const sc_time& other)
    {
        //better would by copy-copnstructor
        m_time = other.m_time;
        m_unit = other.m_unit;
        return *this;
    }
    */


    // conversion functions

    uint64_t sc_time::value() const
    {
        return m_time;
    }

    double sc_time::to_default_time_units() const
    {
        return factor_diff(m_unit, default_time_unit) * m_time;
    }

    uint64_t sc_time::to_smaller_unit(const sc_time_unit other) const
    {
        if(to_factor(m_unit) >= to_factor(other))
        {
            return m_time;
        }
        return m_time * abs_factor_diff(m_unit, other);
    }

    //double to_seconds() const;
    const std::string sc_time::to_string() const
    {
        return std::to_string(m_time) + " " + unit_to_string(m_unit);
    }



    // relational operators

    bool sc_time::operator == ( const sc_time& other ) const
    {
        return to_smaller_unit(other.m_unit) == other.to_smaller_unit(m_unit);
    }


    bool  sc_time::operator != ( const sc_time& other ) const
    {
        return to_smaller_unit(other.m_unit) != other.to_smaller_unit(m_unit);
    }

    bool sc_time::operator <  ( const sc_time& other ) const
    {
        return to_smaller_unit(other.m_unit) < other.to_smaller_unit(m_unit);
    }
    bool sc_time::operator <= ( const sc_time& other) const
    {
        return to_smaller_unit(other.m_unit) <= other.to_smaller_unit(m_unit);
    }
    bool sc_time::operator >  ( const sc_time& other ) const
    {
        return to_smaller_unit(other.m_unit) > other.to_smaller_unit(m_unit);
    }
    bool sc_time::operator >= ( const sc_time& other) const
    {
        return to_smaller_unit(other.m_unit) >= other.to_smaller_unit(m_unit);
    }


    // arithmetic operators

    /*
    sc_time& operator += ( const sc_time& );
    sc_time& operator -= ( const sc_time& );
    */

    const sc_time operator + ( const sc_time& left, const sc_time& right)
    {
        return sc_time(
                left.to_smaller_unit(right.m_unit) +
                right.to_smaller_unit(left.m_unit),
                smallest_unit(left.m_unit, right.m_unit));
    }

    const sc_time operator - ( const sc_time& left, const sc_time& right)
    {
        return sc_time(
                left.to_smaller_unit(right.m_unit) -
                right.to_smaller_unit(left.m_unit),
                smallest_unit(left.m_unit, right.m_unit));
    }

    /*
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

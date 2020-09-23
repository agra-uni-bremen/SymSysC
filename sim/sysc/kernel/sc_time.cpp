#include "sc_time.h"


namespace sc_core
{

    sc_time_unit default_time_unit = SC_US;

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
        return factor_from / factor_to;
    }

    inline sc_time_unit biggest_unit(const sc_time_unit a, const sc_time_unit b)
    {
        return a > b ? a : b;
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
        return m_time * factor_diff(m_unit, default_time_unit);
    }


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
        //double max_factor = max(to_factor(m_unit), to_factor(other.m_unit));
        return to_default_time_units() == other.to_default_time_units();
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

    */
    const sc_time operator + ( const sc_time& left, const sc_time& right)
    {
        sc_time_unit unit = biggest_unit(left.m_unit, right.m_unit);
        return sc_time(
                factor_diff(left.m_unit, unit) * left.m_time +
                factor_diff(right.m_unit, unit) * right.m_time,
                unit);

    }
    const sc_time operator - ( const sc_time& left, const sc_time& right)
    {
        sc_time_unit unit = biggest_unit(left.m_unit, right.m_unit);
        return sc_time(
                factor_diff(left.m_unit, unit) * left.m_time -
                factor_diff(right.m_unit, unit) * right.m_time,
                unit);
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

#pragma once


namespace sc_core {


class sc_module_name
{

public:

    sc_module_name( const char* name) : m_name(name){};
    sc_module_name( const sc_module_name& );

    ~sc_module_name();

    operator const char*() const {return m_name ;};

private:

    const char*     m_name;

private:

    // disabled
    sc_module_name();
    sc_module_name& operator = ( const sc_module_name& );
};

} // namespace sc_core

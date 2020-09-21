#pragma once

#include <inttypes.h>

namespace tlm
{

	//see systemc-dist/include/tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h

	//---------------------------------------------------------------------------
	// enumeration types
	//---------------------------------------------------------------------------
	enum tlm_command {
		TLM_READ_COMMAND,
		TLM_WRITE_COMMAND,
		TLM_IGNORE_COMMAND
	};

	enum tlm_response_status {
		TLM_OK_RESPONSE = 1,
		TLM_INCOMPLETE_RESPONSE = 0,
		TLM_GENERIC_ERROR_RESPONSE = -1,
		TLM_ADDRESS_ERROR_RESPONSE = -2,
		TLM_COMMAND_ERROR_RESPONSE = -3,
		TLM_BURST_ERROR_RESPONSE = -4,
		TLM_BYTE_ENABLE_ERROR_RESPONSE = -5
	};


	struct tlm_generic_payload
	{
	    // Command related method
	    bool                 is_read() const {return (m_command == TLM_READ_COMMAND);}
	    void                 set_read() {m_command = TLM_READ_COMMAND;}
	    bool                 is_write() const {return (m_command == TLM_WRITE_COMMAND);}
	    void                 set_write() {m_command = TLM_WRITE_COMMAND;}
	    tlm_command          get_command() const {return m_command;}
	    void                 set_command(const tlm_command command) {m_command = command;}

	    // Address related methods
	    uint64_t	         get_address() const {return m_address;}
	    void                 set_address(const uint64_t address) {m_address = address;}

	    // Data related methods
	    unsigned char*       get_data_ptr() const {return m_data;}
	    void                 set_data_ptr(unsigned char* data) {m_data = data;}

	    // Transaction length (in bytes) related methods
	    unsigned int         get_data_length() const {return m_length;}
	    void                 set_data_length(const unsigned int length) {m_length = length;}
	private:
	    uint64_t        m_address;
	    tlm_command          m_command;
	    unsigned char*       m_data;
	    unsigned int         m_length;
	    /*
	    //unused
	    tlm_response_status  m_response_status;
	    bool                 m_dmi;
	    unsigned char*       m_byte_enable;
	    unsigned int         m_byte_enable_length;
	    unsigned int         m_streaming_width;
	    */
	    //tlm_gp_option        m_gp_option;
	};
}

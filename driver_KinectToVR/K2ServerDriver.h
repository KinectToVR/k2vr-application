#pragma once
#include <iostream>
#include <zmq.hpp>

class K2ServerDriver
{
	std::string parse_message(std::string message);
	bool _isActive = false;
	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, zmq::socket_type::rep };
	
public:
	int init_ServerDriver(std::string const &port);
	void setActive(bool m_isActive) { _isActive = m_isActive; }
	bool isActive() { return _isActive; }
	
};


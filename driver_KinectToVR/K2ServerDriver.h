#pragma once
#include <iostream>
#include <zmq.hpp>
#include "K2Tracker.h"
#include "easylogging++.h"

class K2ServerDriver
{
	void parse_message(std::string message);
	bool _isActive = false;
	zmq::context_t context{1};
	zmq::socket_t socket{context, zmq::socket_type::rep};

public:
	int init_ServerDriver(std::string const& port);
	void setActive(bool m_isActive) { _isActive = m_isActive; }
	[[nodiscard]] bool isActive() const { return _isActive; }
	std::vector<K2Tracker> trackerVector;
};

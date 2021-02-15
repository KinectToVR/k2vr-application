#pragma once
#include <iostream>
#include <zmq.hpp>
#include "K2Tracker.h"
#include <KinectToVR_API.h>

class K2ServerDriver
{
	// Parse a message from K2API
	void parse_message(std::string message);
	bool _isActive = false; // Server status

	// ZMQ things to work properly
	zmq::context_t context{1};
	zmq::socket_t socket{context, zmq::socket_type::rep};

public:
	int init_ServerDriver(std::string const& port);
	void setActive(bool m_isActive) { _isActive = m_isActive; }

	// Value should not be discarded, it'd be useless
	[[nodiscard]] bool isActive() const { return _isActive; }
	std::vector<K2Tracker> trackerVector;
};

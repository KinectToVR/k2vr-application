#include "K2ServerDriver.h"
#include <thread>

int K2ServerDriver::init_ServerDriver(std::string const& port)
{
    using namespace std::chrono_literals;
    _isActive = true;

	// initialize the zmq context with a single IO thread
    try {
        socket.bind(port);
    }
	catch(std::exception const &e)
	{
        return -1;
	}

    std::thread([&]() {
    	// create chrono for limiting loop timing
        using clock = std::chrono::steady_clock;
        auto next_frame = clock::now();
    	
        // run until termination
        while (true) {
            while (_isActive)
            {
            	// measure loop time, let's run at 140/s
                next_frame += std::chrono::milliseconds(1000 / 140);
            	
            	// create new ZMQ request to receive
                zmq::message_t request;
                // receive a request from client
                socket.recv(&request);
                // parse request, send reply and return
                parse_message(request.msg_str());
            	
				//Sleep until next frame, if time haven't passed yet
                std::this_thread::sleep_until(next_frame); 
            }
            // if we're currently not running, just wait
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }}).detach();

	// if everything went ok, return 0
    return 0;
}

void K2ServerDriver::parse_message(std::string message)
{
    std::string _reply; // Reply that will be sent to client
    bool isReplying = false;
    const std::string cmd("/C"), param("/P"), param1("/P1"), term("/T");
    const std::string _data = message.substr(0, message.rfind(term));

    if (_data.size() != std::string::npos) {

        const std::string _command = _data.substr(_data.find(cmd) + cmd.length(), _data.rfind(param) - param.length()),
            _parameters = _data.substr(_data.find(param) + param.length());

    	// We'll use /P1 switch only with certain commands,
    	// so don't care about it now

    	// Add new tracker to server
        if(_command == "ADD_TRACKER")
        {
            isReplying = true;
        	
        }
    	// Set all trackers' state
        else if(_command == "SET_STATE_ALL")
        {
            isReplying = true;
        	
        }
    	
    	// Rest of commands will need /P1 switch
        std::string _parameter0 = _parameters.substr(0, _parameters.rfind(param1)),
            _parameter1 = _parameters.substr(_parameters.rfind(param1) + param1.length());

    	// Parse only if found more parameters
        if (_parameter0.size() != std::string::npos && _parameter1.size() != std::string::npos) {

            // Set one tracker's state
            if (_command == "SET_STATE")
            {

            }
        	// Update one tracker's pose
            else if (_command == "UPDATE_POSE")
            {

            }
        	// Update one tracker's data: only if it's not initialized yet
            else if (_command == "UPDATE_DATA")
            {

            }
        }

        if (isReplying) {
            // send the reply to the client
            zmq::message_t reply(_reply.size());
            std::memcpy(reply.data(), _reply.data(), _reply.size());
            socket.send(reply);
        }
    }
}

/*
 * FROM-TO
 * BEGIN-STRING_END: data.substr(data.rfind(begin) + begin.length())
 * STRING_BEGIN-END: data.substr(0, data.rfind(end))
 * BEGIN_END: data.substr(data.find(begin) + begin.length(), data.rfind(end) - end.length())
 */
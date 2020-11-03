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
                // parse request and return
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

std::string K2ServerDriver::parse_message(std::string message)
{























	
    std::string data;


	
    // send the reply to the client
    zmq::message_t reply(data.size());
    std::memcpy(reply.data(), data.data(), data.size());
    socket.send(reply);
}


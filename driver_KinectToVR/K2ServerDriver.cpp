#include "K2ServerDriver.h"
#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

int K2ServerDriver::init_ServerDriver(std::string const& port)
{
	using namespace std::chrono_literals;
	_isActive = true;

	// initialize the zmq context with a single IO thread
	try
	{
		socket.bind(port);
	}
	catch (std::exception const& e)
	{
		LOG(ERROR) << "Setting up server failed: " << e.what();
		return -1;
	}

	std::thread([&]()
	{
		// create chrono for limiting loop timing
		using clock = std::chrono::steady_clock;
		auto next_frame = clock::now();
		LOG(INFO) << "Server thread started";

		// run until termination
		while (true)
		{
			while (_isActive)
			{
				// measure loop time, let's run at 140/s
				next_frame += std::chrono::milliseconds(1000 / 140);

				// create new ZMQ request to receive
				zmq::message_t request;

				// receive a request from client
				socket.recv(std::ref(request), zmq::recv_flags::none);
				
				// parse request, send reply and return
				try {
					parse_message(request.to_string());
				}
				catch(std::exception const &e)
				{
					LOG(ERROR) << "Global parsing error: " << e.what();
				}

				//Sleep until next frame, if time haven't passed yet
				std::this_thread::sleep_until(next_frame);
			}
			// if we're currently not running, just wait
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}).detach();

	// if everything went ok, return 0
	return 0;
}

void replace_all(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void K2ServerDriver::parse_message(std::string message)
{
	std::string _reply; // Reply that will be sent to client
	const std::string cmd("/C"), param("/P"), param1("/P1"), term("/T");
	const std::string _data = message.substr(0, message.rfind(term));

	if (!_data.empty())
	{
		std::string _command = _data.substr(_data.find(cmd) + cmd.length(), _data.rfind(param) - param.length()),
		                  _parameters = _data.substr(_data.find(param) + param.length());

		// We'll use /P1 switch only with certain commands,
		// so don't care about it now
		bool oneParamCompleted = false; // did we complete one parameter commands

		// Add new tracker to server
		if (_command == "ADD_TRACKER")
		{
			_reply = "-1"; // Assume fail

			// ZMQ replaces all spaces with 20
			replace_all(_parameters, ",", " ");

			// Create input archive to read parameters
			std::istringstream ifs(_parameters);
			boost::archive::text_iarchive ia(ifs);
			
			// Read parameters (tracker base)
			K2Objects::K2TrackerBase _tracker;
			ia >> _tracker;

			// Check for tracker with same serial
			bool exists_yet = false;
			for(K2Tracker &t: trackerVector)
				if (_tracker.data.serial == t.get_serial())exists_yet = true;

			if (!exists_yet) {
				// Push new tracker to vector and return its id
				trackerVector.emplace_back(K2Tracker(_tracker));
				_reply = std::to_string(trackerVector.size() - 1);

				LOG(INFO) << "New tracker added! Serial: " + _tracker.data.serial +
					" Role: " + ITrackerType_Role_String.at(static_cast<ITrackerType>(_tracker.data.role));
			}
			else LOG(ERROR) << "Couldn't add new tracker. Serial already present.";
			oneParamCompleted = true; // At least 1 command done
		}
			// Set all trackers' state
		else if (_command == "SET_STATE_ALL")
		{
			_reply = "0"; // Assume operation failed

			// Construct bool variable from parameter
			bool _state = boost::lexical_cast<bool>(_parameters);

			// Set all trackers' state
			for (K2Tracker& k2_tracker : trackerVector)
			{
				if (!k2_tracker.is_added()) k2_tracker.spawn(); // spawn if needed
				k2_tracker.set_state(_state); // set state
			}
			LOG(INFO) << "All trackers' state has been set to: " + std::to_string(_state);
			_reply = "1"; // If success, return true
			oneParamCompleted = true; // At least 1 command done
		}

		if (!oneParamCompleted) { // Skip this part if command has run
				// Rest of commands will need /P1 switch

			// ZMQ replaces all spaces with 20
			replace_all(_parameters, ",", " ");
			
			// Remove parameter indicator
			_command = _command.substr(0, _command.rfind(param));
			
			std::string _parameter0 = _parameters.substr(0, _parameters.rfind(param1)),
				_parameter1 = _parameters.substr(_parameters.rfind(param1) + param1.length());

			// Parse only if found more parameters
			if (!_parameter0.empty() && !_parameter1.empty())
			{
				// Set one tracker's state
				if (_command == "SET_STATE")
				{
					_reply = "-1"; // Assume operation failed

					// Construct bool variable from first parameter
					int _id = boost::lexical_cast<int>(_parameter0);

					// Construct bool variable from second parameter
					bool _state = boost::lexical_cast<bool>(_parameter1);

					// Check if desired tracker exists
					if (_id < trackerVector.size()) {
						// Set tracker's state to one gathered from argument
						if (!trackerVector.at(_id).is_added()) trackerVector.at(_id).spawn(); // spawn if needed
						trackerVector.at(_id).set_state(_state);
						_reply = "1"; // If success, return true
						LOG(INFO) << "Tracker id: " + std::to_string(_id) + " state has been set to: " + std::to_string(_state);
					}
					else LOG(ERROR) << "Couldn't set tracker id: " + std::to_string(_id) + " state. Index out of bounds.";
				}
				// Update one tracker's pose
				else if (_command == "UPDATE_POSE")
				{
					_reply = "-1"; // Assume fail
					// Construct bool variable from first parameter
					int _id = boost::lexical_cast<int>(_parameter0);

					// ZMQ replaces all spaces with 20
					replace_all(_parameters, ",", " ");

					// Create input archive from second parameter
					std::istringstream ifs(_parameter1);
					boost::archive::text_iarchive ia(ifs);

					// Read parameters (tracker base)
					K2Objects::K2PosePacket _pose_packet;
					ia >> _pose_packet;

					// Check if desired tracker exists
					if (_id < trackerVector.size()) {
						// Update tracker pose (with time offset)
						if (int(_pose_packet.millisFromNow) != 0)
						{
							trackerVector.at(_id).set_pose(_pose_packet, _pose_packet.millisFromNow);
						}
						else
						{
							// If there is no time offset, just update
							trackerVector.at(_id).set_pose(_pose_packet);
						}
						_reply = std::to_string(_id); // If success, return tracker id
					}
					else LOG(ERROR) << "Couldn't set tracker id: " + std::to_string(_id) + " pose. Index out of bounds.";
				}
				// Update one tracker's data: only if it's not initialized yet
				else if (_command == "UPDATE_DATA")
				{
					_reply = "-1"; // Assume fail
					// Construct bool variable from first parameter
					int _id = boost::lexical_cast<int>(_parameter0);

					// ZMQ replaces all spaces with 20
					replace_all(_parameters, ",", " ");

					// Create input archive from second parameter
					std::istringstream ifs(_parameter1);
					boost::archive::text_iarchive ia(ifs);

					// Read parameters (tracker base)
					K2Objects::K2DataPacket _data_packet;
					ia >> _data_packet;

					// Check for tracker with same serial
					bool exists_yet = false;
					for (K2Tracker& t : trackerVector)
						if (_data_packet.serial == t.get_serial())exists_yet = true;

					if (!exists_yet) {
						// Check if desired tracker exists
						if (_id < trackerVector.size()) {
							// Update tracker pose (with time offset)
							if (int(_data_packet.millisFromNow) != 0)
							{
								trackerVector.at(_id).set_data(_data_packet, _data_packet.millisFromNow);
							}
							else
							{
								// If there is no time offset, just update
								trackerVector.at(_id).set_data(_data_packet);
							}
							_reply = std::to_string(_id); // If success, return tracker id
						}
						else LOG(ERROR) << "Couldn't set tracker id: " + std::to_string(_id) + " data. Index out of bounds.";
					}
					else LOG(ERROR) << "Couldn't set tracker id: " + std::to_string(_id) + " data. Serial already present.";
				}
			}
		}

		// send the reply to the client
		zmq::message_t reply(_reply.size());
		std::memcpy(reply.data(), _reply.data(), _reply.size());
		socket.send(reply, zmq::send_flags::none);
	}
}

/*
 * FROM-TO
 * BEGIN-STRING_END: data.substr(data.rfind(begin) + begin.length())
 * STRING_BEGIN-END: data.substr(0, data.rfind(end))
 * BEGIN_END: data.substr(data.find(begin) + begin.length(), data.rfind(end) - end.length())
 */

#include "K2ServerDriver.h"
#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

int K2ServerDriver::init_ServerDriver(
	const std::string& k2_to_pipe,
	const std::string& k2_from_pipe,
	const std::string& k2_to_sem,
	const std::string& k2_from_sem,
	const std::string& k2_start_sem)
{
	using namespace std::chrono_literals;
	_isActive = true;

	// initialize the semaphores
	try
	{
		// Create the *to* semaphore
		k2api_to_Semaphore = CreateSemaphoreA(
			NULL, //Security Attributes
			0,	  //Initial State i.e.Non Signaled
			1,    //No. of Resources
			"Global\\k2api_to_sem");//Semaphore Name

		if (nullptr == k2api_to_Semaphore)
		{
			LOG(ERROR) << "Semaphore Creation Failed\n";
			LOG(ERROR) << "Error No - " << GetLastError() << '\n';
			return -1;
		}
		LOG(INFO) << "*TO* Semaphore Creation Success\n";

		// Automatically release the sem after creation
		ReleaseSemaphore(k2api_to_Semaphore, 1, 0);

		// Create the *from* semaphore
		k2api_from_Semaphore = CreateSemaphoreA(
			NULL, //Security Attributes
			0,    //Initial State i.e.Non Signaled
			1,    //No. of Resources
			"Global\\k2api_from_sem");//Semaphore Name

		if (nullptr == k2api_from_Semaphore)
		{
			LOG(ERROR) << "Semaphore Creation Failed\n";
			LOG(ERROR) << "Error No - " << GetLastError() << '\n';
			return -1;
		}
		LOG(INFO) << "*FROM* Semaphore Creation Success\n";

		// Create the *start* semaphore
		k2api_start_Semaphore = CreateSemaphoreA(
			NULL, //Security Attributes
			0,    //Initial State i.e.Non Signaled
			1,    //No. of Resources
			"Global\\k2api_start_sem");//Semaphore Name

		if (nullptr == k2api_start_Semaphore)
		{
			LOG(ERROR) << "Semaphore Creation Failed\n";
			LOG(ERROR) << "Error No - " << GetLastError() << '\n';
			return -1;
		}
		LOG(INFO) << "*START* Semaphore Creation Success\n";
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

					// Wait for the client to request a read
					while (WaitForSingleObject(k2api_start_Semaphore, 15000L) != WAIT_OBJECT_0) {
						LOG(INFO) << "Releasing the *TO* semaphore\n";
						// Release the semaphore in case something hangs,
						// no request would take as long as 15 seconds anyway
						ReleaseSemaphore(k2api_to_Semaphore, 1, 0);
					}

					// Here, read from the *TO* pipe
					// Create the pipe file
					std::optional<HANDLE> ReaderPipe = CreateFile(
						TEXT("\\\\.\\pipe\\k2api_to_pipe"),
						GENERIC_READ | GENERIC_WRITE,
						0, nullptr, OPEN_EXISTING, 0, nullptr);

					// Create the buffer
					char read_buffer[1024];
					DWORD Read = DWORD();
					std::string read_string;

					// Check if we're good
					if (ReaderPipe.has_value()) {

						// Read the pipe
						ReadFile(ReaderPipe.value(),
							read_buffer, 1024,
							&Read, nullptr);

						// Convert the message to string
						read_string = read_buffer;
					}
					else
						LOG(ERROR) << "Error: Pipe object was not initialized.";

					// Close the pipe
					CloseHandle(ReaderPipe.value());
					
					// parse request, send reply and return
					try {
						// I don't know why, when and how,
						// but somehow K2API inserts this shit at index 62
						std::string _s = read_string;
						if (_s.find("3120300a") == 62)_s.erase(62, 8);

						// Convert hex to readable ascii and parse
						std::string s = ktvr::asciiString(_s);
						parse_message(ktvr::K2Message::parseFromString(s));
					}
					catch (boost::archive::archive_exception const& e)
					{
						LOG(ERROR) << "Message may be corrupted. Boost serialization error: " << e.what();
					}
					catch (std::exception const& e)
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

void K2ServerDriver::parse_message(const ktvr::K2Message& message)
{
	std::string _reply; // Reply that will be sent to client
	ktvr::K2ResponseMessage _response; // Response message to be sent
	
	// Add the timestamp: parsing
	_response.messageManualTimestamp = K2API_GET_TIMESTAMP_NOW;

	// Parse the message if it's not invalid
	if (message.messageType != ktvr::K2MessageType::K2Message_Invalid) {

		// Switch based on the message type
		switch (message.messageType) {

		case ktvr::K2MessageType::K2Message_AddTracker: {
			// Check if the serial is okay
			if (!message.tracker_base.data.serial.empty()) {
				// Check for tracker with same serial
				bool exists_yet = false;
				for (K2Tracker& t : trackerVector)
					if (message.tracker_base.data.serial == t.get_serial())
						exists_yet = true; // Already exists

				if (!exists_yet) {
					// Push new tracker to vector and return its id
					trackerVector.emplace_back(K2Tracker(message.tracker_base));

					LOG(INFO) << "New tracker added! Serial: " + message.tracker_base.data.serial +
						" Role: " + ktvr::ITrackerType_Role_String.at(static_cast<ktvr::ITrackerType>(message.tracker_base.data.role));

					if (message.tracker_base.data.isActive && // If autospawn is desired
						!trackerVector.at(message.id).is_added()) {

						// Spawn and set the state
						if (trackerVector.at(message.id).spawn()) {
							trackerVector.at(message.id).set_state(
								message.tracker_base.data.isActive);

							LOG(INFO) << "Tracker autospawned! Serial: " + message.tracker_base.data.serial +
								" Role: " + ktvr::ITrackerType_Role_String.at(static_cast<ktvr::ITrackerType>(message.tracker_base.data.role));
						}
						else {
							LOG(INFO) << "Tracker autospawn exception! Serial: " + message.tracker_base.data.serial +
								" Role: " + ktvr::ITrackerType_Role_String.at(static_cast<ktvr::ITrackerType>(message.tracker_base.data.role));
							_response.result = ktvr::K2ResponseMessageCode_SpawnFailed;
						}
					}

					// Compose the response
					_response.success = true;
					_response.id = (int)trackerVector.size() - 1; // ID

					// Copy the tracker base object
					_response.tracker_base = trackerVector.back().getTrackerBase();
					_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_Tracker;
				}
				else {
					LOG(ERROR) << "Couldn't add new tracker. Serial already present.";
					_response.result = ktvr::K2ResponseMessageCode_AlreadyPresent;
				}
			}
			else {
				LOG(ERROR) << "Couldn't add new tracker. Serial is empty.";
				_response.result = ktvr::K2ResponseMessageCode_BadSerial;
			}
		}break;

		case ktvr::K2MessageType::K2Message_SetTrackerState: {
			// Check if desired tracker exists
			if (message.id < trackerVector.size() && message.id >= 0) {

				// Set tracker's state to one gathered from argument
				if (!trackerVector.at(message.id).is_added())
					if (!trackerVector.at(message.id).spawn()) { // spawn if needed
						LOG(INFO) << "Tracker autospawn exception! Serial: " + message.tracker_base.data.serial +
							" Role: " + ktvr::ITrackerType_Role_String.at(static_cast<ktvr::ITrackerType>(message.tracker_base.data.role));
						_response.result = ktvr::K2ResponseMessageCode_SpawnFailed;
					}

				// Set the state
				trackerVector.at(message.id).set_state(message.state);
				LOG(INFO) << "Tracker id: " + std::to_string(message.id) +
					" state has been set to: " + std::to_string(message.state);

				// Compose the response
				_response.success = true;
				_response.id = message.id; // ID
				_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_ID;
			}
			else {
				LOG(ERROR) << "Couldn't set tracker id: " +
					std::to_string(message.id) + " state. Index out of bounds.";
				_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			}
		}break;

		case ktvr::K2MessageType::K2Message_SetStateAll: {
			// Set all trackers' state
			for (K2Tracker& k2_tracker : trackerVector)
			{
				if (!k2_tracker.is_added())
					if (!k2_tracker.spawn()) { // spawn if needed
						LOG(INFO) << "Tracker autospawn exception! Serial: " + message.tracker_base.data.serial +
							" Role: " + ktvr::ITrackerType_Role_String.at(static_cast<ktvr::ITrackerType>(message.tracker_base.data.role));
						_response.result = ktvr::K2ResponseMessageCode_SpawnFailed;
					}
				k2_tracker.set_state(message.state); // set state
			}
			LOG(INFO) << "All trackers' state has been set to: " + std::to_string(message.state);

			// Compose the response
			_response.success = true;
			_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_Success;
		}break;

		case ktvr::K2MessageType::K2Message_UpdateTrackerPose: {
			// Check if desired tracker exists
			if (message.id < trackerVector.size() && message.id >= 0) {
				// Update tracker pose (with time offset)
				trackerVector.at(message.id).set_pose(message.tracker_pose);

				// Compose the response
				_response.success = true;
				_response.id = message.id; // ID
				_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_ID;
			}
			else {
				LOG(ERROR) << "Couldn't set tracker id: " +
					std::to_string(message.id) + " pose. Index out of bounds.";
				_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			}
		}break;

		case ktvr::K2MessageType::K2Message_UpdateTrackerData: {
			// Check if desired tracker exists
			if (message.id < trackerVector.size() && message.id >= 0) {
				// Update tracker data (with time offset)
				trackerVector.at(message.id).set_data(message.tracker_data);

				// Compose the response
				_response.success = true;
				_response.id = message.id; // ID
				_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_ID;
			}
			else {
				LOG(ERROR) << "Couldn't set tracker id: " +
					std::to_string(message.id) + " data. Index out of bounds.";
				_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			}
		}break;

		case ktvr::K2MessageType::K2Message_DownloadTracker: {
			// Check if desired tracker exists
			if (message.id < trackerVector.size() && message.id >= 0) {
				// Copy the tracker object
				_response.tracker_base =
					trackerVector.at(message.id).getTrackerBase();

				// Compose the response
				_response.success = true;
				_response.id = message.id; // ID
				_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_Tracker;
			}
			// In case we're searching with serial
			else if (message.id < 0
				&& !message.tracker_data.serial.empty()) {

				bool trackerFound = false;

				// If we can even search
				if (!trackerVector.empty()) {
					
					// Search in our trackers vector
					for (int _tracker_id = 0; _tracker_id < trackerVector.size(); _tracker_id++)
					{
						// If we've found the one
						if (message.tracker_data.serial == trackerVector.at(_tracker_id).get_serial())
						{
							// Copy the tracker object
							_response.tracker_base = trackerVector.at(_tracker_id).getTrackerBase();

							// Compose the response
							_response.success = true;
							_response.id = _tracker_id; // Return the ID
							_response.messageType = ktvr::K2ResponseMessageType::K2ResponseMessage_Tracker;

							LOG(INFO) << "Serial: " + trackerVector.at(_tracker_id).get_serial() + ", id: " + std::to_string(_response.id);

							// Exit loop
							trackerFound = true;
							break;
						}
					}
				}
				else
				{
					// If tracker was not found
					LOG(ERROR) << "Couldn't download tracker via serial: " +
						message.tracker_data.serial + " because there are no trackers added.";
					_response.result = ktvr::K2ResponseMessageCode_BadRequest;
				}

				// If tracker was not found
				if (!trackerFound)
					LOG(ERROR) << "Couldn't download tracker via serial: " +
					message.tracker_data.serial + ". Not found.";
				_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			}
			else {
				LOG(ERROR) << "Couldn't download tracker via id: " +
					std::to_string(message.id) + " or serial. Index out of bounds / empty.";
				_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			}
		}break;
		case ktvr::K2MessageType::K2Message_Ping: {
			// Compose the response
			_response.success = true;
			_response.messageType =
				ktvr::K2ResponseMessageType::K2ResponseMessage_Tracker;
		}break;
		default:
			LOG(ERROR) << "Couldn't process message. The message type was not set. (Type invalid)";
			_response.result = ktvr::K2ResponseMessageCode_BadRequest;
			break;
		}
	}
	else {
		LOG(ERROR) << "Couldn't process message. Message had had invalid type.";
		_response.result = ktvr::K2ResponseMessageCode_ParsingError;
	}

	// Check the return code
	if (_response.success) // If succeed, let's sassume it's okay
		_response.result = ktvr::K2ResponseMessageCode_OK;

	// Set the manual timestamp
	_response.messageTimestamp = K2API_GET_TIMESTAMP_NOW;

	// Serialize the response
	_reply = // Serialize to hex format
		ktvr::hexString(_response.serializeToString());

	// Check if the client wants a response and eventually send it
	if(message.want_reply)
	{
		// Here, write to the *from* pipe
		HANDLE WriterPipe = CreateNamedPipe(
			TEXT("\\\\.\\pipe\\k2api_from_pipe"),
			PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			1, 1024, 1024, 1000L, nullptr);
		DWORD Written;
		
		// Let the client know that we'll be writing soon
		ReleaseSemaphore(k2api_from_Semaphore, 1, 0);

		// Read from the pipe
		ConnectNamedPipe(WriterPipe, nullptr);
		WriteFile(WriterPipe,
			_reply.c_str(),
			strlen(_reply.c_str()),
			&Written, nullptr);
		FlushFileBuffers(WriterPipe);

		// Disconnect and close the pipe
		DisconnectNamedPipe(WriterPipe);
		CloseHandle(WriterPipe);
	}
}

/*
 * FROM-TO
 * BEGIN-STRING_END: data.substr(data.rfind(begin) + begin.length())
 * STRING_BEGIN-END: data.substr(0, data.rfind(end))
 * BEGIN_END: data.substr(data.find(begin) + begin.length(), data.rfind(end) - end.length())
 */

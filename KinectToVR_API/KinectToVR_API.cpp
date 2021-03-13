#include "pch.h"
#include "KinectToVR_API.h"

void replace_all(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

namespace ktvr
{
	int init_socket(const int port) noexcept
	{
		try
		{
			// Connect to the desired port via tcp, on localhost (127.0.0.1 should be working too)
			socket.connect("tcp://localhost:" + std::to_string(port));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
		return 0;
	}

	int close_socket() noexcept
	{
		try
		{
			// Otherwise we'll get WSASTARTUP errors
			socket.close();
			context.close();
		}
		catch (std::exception const& e)
		{
			return -1;
		}
		return 0;
	}

	std::string send_message(std::string const& data) noexcept(false)
	{
		// change the string to hex format
		std::string msg_data = hexString(data);

		// construct new ZMQ message and copy data to it
		zmq::message_t message(msg_data.size());
		std::memcpy(message.data(), msg_data.data(), msg_data.size());
		socket.send(message, zmq::send_flags::none); // send

		// wait for reply from server
		zmq::message_t reply{};
		socket.recv(std::ref(reply), zmq::recv_flags::none);

		// I don't know why, when and how,
		// but somehow K2API inserts this shit at index 62
		std::string _s = reply.to_string();
		if (_s.find("3120300a") == 62)_s.erase(62, 8);

		// decode hex reply
		std::string reply_data = asciiString(_s);
		return reply_data; // Return only the reply
	}

	K2ResponseMessage send_message(K2Message message) noexcept(false)
	{
		// Serialize to string
		std::ostringstream o;
		boost::archive::text_oarchive oa(o);
		oa << message;

		// send the message
		std::string reply =
			send_message(o.str());

		// Deserialize the response
		K2ResponseMessage response;
		std::istringstream i(reply);
		boost::archive::text_iarchive ia(i);
		ia >> response;

		// Deserialize reply and return
		return response;
	}

	K2ResponseMessage add_tracker(K2TrackerBase& tracker) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			auto response = send_message(
				K2Message(tracker));

			// Overwrite the current tracker's id
			tracker.id = response.id;

			// Send the message and return
			return response;
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage set_tracker_state(int id, bool state) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message(
				K2Message(id, state));
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage set_state_all(bool state) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message(
				K2Message(state));
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage update_tracker_pose(int id, K2PosePacket const& tracker_pose) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message(
				K2Message(id, tracker_pose));
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage update_tracker_pose(K2TrackerBase const& tracker_handle) noexcept
	{
		try
		{
			// Send the message and return
			return update_tracker_pose(tracker_handle.id, tracker_handle.pose);
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage update_tracker_data(int id, K2DataPacket const& tracker_data) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message(
				K2Message(id, tracker_data));
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage update_tracker_data(K2TrackerBase const& tracker_handle) noexcept
	{
		try
		{
			// Send the message and return
			return update_tracker_data(tracker_handle.id, tracker_handle.data);
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage update_tracker(K2TrackerBase const& tracker) noexcept
	{
		try
		{
			// Send the message and return
			update_tracker_pose(tracker.id, tracker.pose);

			// Data is more important then return data
			return update_tracker_data(tracker.id, tracker.data);
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage download_tracker(int const& tracker_id) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message(
				K2Message(tracker_id));
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	K2ResponseMessage download_tracker(K2TrackerBase const& tracker) noexcept
	{
		try
		{
			// Send the message and return
			return download_tracker(tracker.id);
		}
		catch (std::exception const& e)
		{
			return K2ResponseMessage(); // Success is set to false by default
		}
	}

	std::string hexString(const std::string& s)
	{
		std::ostringstream ret;

		// Change ascii string to hex string
		for (char c : s)
			ret << std::hex << std::setfill('0') << std::setw(2) << std::nouppercase << (int)c;
		std::string ret_s = ret.str();

		// Erase 0a00 if there is so
		if (ret_s.find("0a00") == 0)ret_s.erase(0, 4);
		return ret_s;;
	}

	std::string asciiString(const std::string& s)
	{
		std::ostringstream ret;

		// Start from the index 2, removing 0A00 occur
		for (std::string::size_type i = 0; i < s.length(); i += 2)
			ret << (char)(int)strtol(s.substr(i, 2).c_str(), nullptr, 16);

		return ret.str();
	}
}

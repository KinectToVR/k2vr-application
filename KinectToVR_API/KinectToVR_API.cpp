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

namespace k2_api
{
	int init_socket(const int port) noexcept
	{
		try
		{
			socket.connect("tcp://localhost:" + std::to_string(port));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
		return 0;
	}

	int add_tracker(K2Objects::K2TrackerBase const& tracker) noexcept
	{
		try
		{
			// pack tracker object into text archive
			std::ostringstream ofs;
			boost::archive::text_oarchive out(ofs);
			out << tracker;

			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "ADD_TRACKER" "/P" + ofs.str() + "/T";

			// return what we got - assuming last tracker's id
			return boost::lexical_cast<int>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int add_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept
	{
		try
		{
			int ret = -1;
			for (const K2Objects::K2TrackerBase tracker : tracker_vector)
				ret = add_tracker(tracker);
			return ret;
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int add_and_connect_tracker(K2Objects::K2TrackerBase const& tracker) noexcept
	{
		try
		{
			add_tracker(tracker);
			return connect_tracker(tracker.id);
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int add_and_connect_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept
	{
		try
		{
			add_tracker(tracker_vector);
			int ret = -1;
			for (const K2Objects::K2TrackerBase tracker : tracker_vector)
				ret = connect_tracker(tracker.id);
			return ret;
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int connect_tracker(const int id) noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE" "/P" + std::to_string(id) + "/P1" "1/T";

			// return what we got - assuming last tracker's id
			return boost::lexical_cast<int>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int disconnect_tracker(const int id) noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE" "/P" + std::to_string(id) + "/P1" "0/T";

			// return what we got - assuming last tracker's id
			return boost::lexical_cast<int>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	bool connect_all_trackers() noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE_ALL" "/P" "1/T";

			// return what we got - assuming 0/1
			return boost::lexical_cast<bool>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return false;
		}
	}

	bool disconnect_all_trackers() noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE_ALL" "/P" "0/T";

			// return what we got - assuming 0/1
			return boost::lexical_cast<bool>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return false;
		}
	}

	int update_tracker_pose(const int id, K2Objects::K2PosePacket const& tracker_pose) noexcept
	{
		try
		{
			// pack tracker object into text archive
			std::ostringstream ofs;
			boost::archive::text_oarchive out(ofs);
			out << tracker_pose;

			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "UPDATE_POSE" "/P" + std::to_string(id) + "/P1" + ofs.str() + "/T";

			// return what we got - assuming int
			return boost::lexical_cast<int>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	int update_tracker_data(const int id, K2Objects::K2DataPacket const& tracker_data) noexcept
	{
		try
		{
			// pack tracker object into text archive
			std::ostringstream ofs;
			boost::archive::text_oarchive out(ofs);
			out << tracker_data;

			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "UPDATE_DATA" "/P" + std::to_string(id) + "/P1" + ofs.str() + "/T";

			// return what we got - assuming int
			return boost::lexical_cast<int>(send_message(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	std::string send_message(std::string data) noexcept(false)
	{
		// Replace all spaces into normal characters
		replace_all(data, " ", ",");
		
		// construct new ZMQ message and copy data to it
		zmq::message_t message(data.size());
		std::memcpy(message.data(), data.data(), data.size());
		socket.send(message, zmq::send_flags::none); // send
		
		// wait for reply from server
		zmq::message_t reply{};
		socket.recv(std::ref(reply), zmq::recv_flags::none);

		return reply.to_string();
	}
}

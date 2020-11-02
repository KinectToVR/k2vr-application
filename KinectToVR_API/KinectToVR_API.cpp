#include "pch.h"
#include "KinectToVR_API.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <random>

namespace k2_api
{
	/**
	 * \brief Connects socket object to selected port
	 * \param port TCP port on which should be all calls created
	 * \return Returns 0 for success and -1 for failure
	 */
	int init_socket(std::string const& port) noexcept
	{
		try
		{
			socket.connect("tcp://localhost:" + port);
		}
		catch (std::exception const& e)
		{
			return -1;
		}
		return 0;
	}

	/**
	 * \brief Add tracker to driver's trackers list
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker's id or -1 for failure
	 */
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
			return boost::lexical_cast<int>(send_message_r(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	/**
	 * \brief Add vector of trackers to driver's tracker list
	 * \param tracker_vector Vector of trackers to add to driver
	 * \return Returns last tracker's id or -1 for failure
	 */
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

	/**
	 * \brief Add tracker to driver's trackers list and connect (spawn) it right away
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker's id or -1 for failure
	 */
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

	/**
	 * \brief Add vector of trackers to driver's tracker list and connect (spawn) it right away
	 * \param tracker_vector Vector of trackers to add to driver
	 * \return Returns last tracker's id or -1 for failure
	 */
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

	/**
	 * \brief Connect (activate/spawn) tracker in SteamVR
	 * \param id Tracker's id which is to connect
	 * \return Returns connected tracker's id or -1 for failure
	 */
	int connect_tracker(const int id) noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE" "/P" + std::to_string(id) + ";1/T";

			// return what we got - assuming last tracker's id
			return boost::lexical_cast<int>(send_message_r(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	/**
	 * \brief Disconnect (deactivate) tracker in SteamVR
	 * \param id Tracker's id which is to disconnect
	 * \return Returns disconnected tracker's id or -1 for failure
	 */
	int disconnect_tracker(const int id) noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE" "/P" + std::to_string(id) + ";0/T";

			// return what we got - assuming last tracker's id
			return boost::lexical_cast<int>(send_message_r(data));
		}
		catch (std::exception const& e)
		{
			return -1;
		}
	}

	/**
	 * \brief Connect (activate/spawn) all trackers in SteamVR
	 * \return Returns true for success and false for failure
	 */
	bool connect_all_trackers() noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE_ALL" "/P" "1/T";

			// return what we got - assuming 0/1
			return boost::lexical_cast<bool>(send_message_r(data));
		}
		catch (std::exception const& e)
		{
			return false;
		}
	}

	/**
	 * \brief Disconnect (deactivate) all trackers in SteamVR
	 * \return Returns true for success and false for failure
	 */
	bool disconnect_all_trackers() noexcept
	{
		try
		{
			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "SET_STATE_ALL" "/P" "0/T";

			// return what we got - assuming 0/1
			return boost::lexical_cast<bool>(send_message_r(data));
		}
		catch (std::exception const& e)
		{
			return false;
		}
	}

	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param id Tracker's id which is to update
	 * \param tracker_pose New pose for tracker
	 */
	void update_tracker_pose(const int id, K2Objects::K2PosePacket const& tracker_pose) noexcept
	{
		try
		{
			// pack tracker object into text archive
			std::ostringstream ofs;
			boost::archive::text_oarchive out(ofs);
			out << tracker_pose;

			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "UPDATE_POSE" "/P" + std::to_string(id) + ";" + ofs.str() + "/T";

			send_message_nr(data);
			// don't check or return - we need to go fast
		}
		catch (std::exception const& e)
		{
		}
	}

	/**
	 * \brief Update tracker's data in SteamVR driver (ONLY for yet not spawned trackers)
	 * \param id Tracker's id which is to update
	 * \param tracker_data New pose for tracker
	 */
	void update_tracker_data(const int id, K2Objects::K2DataPacket const& tracker_data) noexcept
	{
		try
		{
			// pack tracker object into text archive
			std::ostringstream ofs;
			boost::archive::text_oarchive out(ofs);
			out << tracker_data;

			// generate data for server with arguments /C for command and /P for parameters
			const std::string data = "/C" "UPDATE_DATA" "/P" + std::to_string(id) + ";" + ofs.str() + "/T";

			send_message_nr(data);
			// don't check or return - we need to go fast
		}
		catch (std::exception const& e)
		{
		}
	}

	/**
	 * \brief Send message via ZMQ and get a server reply
	 * \param data String which is to send
	 * \return Returns server's reply to message
	 */
	std::string send_message_r(std::string const& data) noexcept(false)
	{
		// construct new ZMQ message and copy data to it
		zmq::message_t message(data.size());
		std::memcpy(message.data(), data.data(), data.size());
		socket.send(message); // send

		// wait for reply from server
		zmq::message_t reply{};
		socket.recv(&reply);

		return reply.msg_str();
	}

	/**
	 * \brief Send message via ZMQ and get a server reply
	 * \param data String which is to send
	 */
	void send_message_nr(std::string const& data) noexcept(false)
	{
		// construct new ZMQ message and copy data to it
		zmq::message_t message(data.size());
		std::memcpy(message.data(), data.data(), data.size());
		socket.send(message); // send
	}
}

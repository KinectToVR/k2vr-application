#pragma once
#include <iostream>
#include <vector>
#include "K2Objects.h"
#include "zmq.hpp"

#ifdef KINECTTOVR_API_EXPORTS
#define KINECTTOVR_API __declspec(dllexport)
#else
#define KINECTTOVR_API __declspec(dllimport)
#endif

namespace k2_api
{
	inline zmq::context_t context{1};
	inline zmq::socket_t socket{context, zmq::socket_type::req};

	/**
	 * \brief Connects socket object to selected port
	 * \param port TCP port on which should be all calls created
	 * \return Returns 0 for success and -1 for failure
	 */
	KINECTTOVR_API int init_socket(int port) noexcept;

	/**
	 * \brief Add tracker to driver's trackers list
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker's id or -1 for failure
	 */
	KINECTTOVR_API int add_tracker(K2Objects::K2TrackerBase const& tracker) noexcept;
	/**
	 * \brief Add vector of trackers to driver's tracker list
	 * \param tracker_vector Vector of trackers to add to driver
	 * \return Returns last tracker's id or -1 for failure
	 */
	KINECTTOVR_API int add_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept;

	/**
	 * \brief Add tracker to driver's trackers list and connect (spawn) it right away
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker's id or -1 for failure
	 */
	KINECTTOVR_API int add_and_connect_tracker(K2Objects::K2TrackerBase const& tracker) noexcept;
	/**
	 * \brief Add vector of trackers to driver's tracker list and connect (spawn) it right away
	 * \param tracker_vector Vector of trackers to add to driver
	 * \return Returns last tracker's id or -1 for failure
	 */
	KINECTTOVR_API int add_and_connect_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept;

	/**
	 * \brief Connect (activate/spawn) tracker in SteamVR
	 * \param id Tracker's id which is to connect
	 * \return Returns connected tracker's id or -1 for failure
	 */
	KINECTTOVR_API int connect_tracker(int id) noexcept;
	 /**
	  * \brief Connect (activate/spawn) all trackers in SteamVR
	  * \return Returns true for success and false for failure
	  */
	KINECTTOVR_API bool connect_all_trackers() noexcept;
	/**
	 * \brief Disconnect (deactivate) tracker in SteamVR
	 * \param id Tracker's id which is to disconnect
	 * \return Returns disconnected tracker's id or -1 for failure
	 */
	KINECTTOVR_API int disconnect_tracker(int id) noexcept;
	/**
	 * \brief Disconnect (deactivate) all trackers in SteamVR
	 * \return Returns true for success and false for failure
	 */
	KINECTTOVR_API bool disconnect_all_trackers() noexcept;

	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param id Tracker's id which is to update
	 * \param tracker_pose New pose for tracker
	 * \return Returns tracker's id or -1 for failure
	 */
	KINECTTOVR_API int update_tracker_pose(int id, K2Objects::K2PosePacket const& tracker_pose) noexcept;
	/**
	 * \brief Update tracker's data in SteamVR driver (ONLY for yet not spawned trackers)
	 * \param id Tracker's id which is to update
	 * \param tracker_data New pose for tracker
	 * \return Returns tracker's id or -1 for failure
	 */
	KINECTTOVR_API int update_tracker_data(int id, K2Objects::K2DataPacket const& tracker_data) noexcept;

	/**
	 * \brief Send message via ZMQ and get a server reply
	 * \param data String which is to send
	 * \return Returns server's reply to message
	 */
	std::string send_message(std::string const& data) noexcept(false);
}

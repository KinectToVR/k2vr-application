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
	KINECTTOVR_API int init_socket(int port) noexcept;

	KINECTTOVR_API int add_tracker(K2Objects::K2TrackerBase const& tracker) noexcept;
	KINECTTOVR_API int add_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept;

	KINECTTOVR_API int add_and_connect_tracker(K2Objects::K2TrackerBase const& tracker) noexcept;
	KINECTTOVR_API int add_and_connect_tracker(std::vector<K2Objects::K2TrackerBase> const& tracker_vector) noexcept;

	KINECTTOVR_API int connect_tracker(int id) noexcept;
	KINECTTOVR_API bool connect_all_trackers() noexcept;
	KINECTTOVR_API int disconnect_tracker(int id) noexcept;
	KINECTTOVR_API bool disconnect_all_trackers() noexcept;

	KINECTTOVR_API void update_tracker_pose(int id, K2Objects::K2PosePacket const& tracker_pose) noexcept;
	KINECTTOVR_API void update_tracker_data(int id, K2Objects::K2DataPacket const& tracker_data) noexcept;

	std::string send_message_r(std::string const& data) noexcept(false);
	void send_message_nr(std::string const& data) noexcept(false);
}

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

namespace K2API {

	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, zmq::socket_type::req };
	KINECTTOVR_API int initSocket(std::string port);
	
	KINECTTOVR_API int addTracker(K2Objects::K2TrackerBase tracker);
	KINECTTOVR_API int addTracker(std::vector<K2Objects::K2TrackerBase> trackerVector);

	KINECTTOVR_API int addAndConnectTracker(K2Objects::K2TrackerBase tracker);
	KINECTTOVR_API int addAndConnectTracker(std::vector<K2Objects::K2TrackerBase> trackerVector);

	KINECTTOVR_API int connectTracker(int id) noexcept(false);
	KINECTTOVR_API bool connectAllTrackers() noexcept(false);
	KINECTTOVR_API int disconnectTracker(int id) noexcept(false);
	KINECTTOVR_API bool disconnectAllTrackers() noexcept(false);

	KINECTTOVR_API void updateTrackerPose(int id, K2Objects::K2PosePacket trackerPose);
	KINECTTOVR_API void updateTrackerData(int id, K2Objects::K2DataPacket trackerData);
}

#pragma once
#include <iostream>
#include <vector>
#include "K2Objects.h"

#ifdef KINECTTOVR_API_EXPORTS
#define KINECTTOVR_API __declspec(dllexport)
#else
#define KINECTTOVR_API __declspec(dllimport)
#endif

namespace K2API {

	int serverFramerate = 90;
	void sendMessage(std::string command, K2Objects::K2TrackerBase tracker, uint32_t hash) noexcept;
	void sendMessage(std::string command, K2Objects::K2PosePacket trackerPose) noexcept;
	void sendMessage(std::string command, K2Objects::K2DataPacket trackerData) noexcept;
	void sendMessage(std::string command, int arg, uint32_t hash) noexcept;
	int getServerReply(uint32_t hash) noexcept(false);

	typedef int (*function_callback)(K2Objects::K2TrackerBase);
	typedef int (*function_callback_pose)(K2Objects::K2PosePacket);
	typedef int (*function_callback_data)(K2Objects::K2DataPacket);
	typedef int (*function_callback_int)(int);

	function_callback trackerAddedCallback;
	function_callback_pose poseUpdatedCallback;
	function_callback_data dataUpdatedCallback;

	function_callback_int trackerConnectedCallback;
	function_callback_int trackerDisconnectedCallback;

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

	KINECTTOVR_API int setTrackerAddedCallback(function_callback callback);
	KINECTTOVR_API int setPoseUpdatedCallback(function_callback_pose callback);
	KINECTTOVR_API int setDataUpdatedCallback(function_callback_data callback);

	KINECTTOVR_API int setTrackerConnectedCallback(function_callback_int callback);
	KINECTTOVR_API int setTrackerDisconnectedCallback(function_callback_int callback);

	KINECTTOVR_API void startListeningServer();
	KINECTTOVR_API void setServerFramerate(int fps);

	KINECTTOVR_API uint32_t getRandHash();
}

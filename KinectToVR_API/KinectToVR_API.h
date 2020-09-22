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

	std::vector<K2Objects::K2TrackerBase> trackerList;
	int serverFramerate = 90;
	void sendMessage(std::string command, K2Objects::K2TrackerBase tracker);
	void sendMessage(std::string command, std::vector<K2Objects::K2TrackerBase> trackerVector);
	void sendMessage(std::string command, int arg);
	typedef int (*function_callback)(K2Objects::K2TrackerBase);

	function_callback trackerAddedCallback;
	function_callback poseUpdatedCallback;
	function_callback dataUpdatedCallback;


	KINECTTOVR_API void setServerFramerate(int fps);


	KINECTTOVR_API int addTracker(K2Objects::K2TrackerBase tracker);
	KINECTTOVR_API int addTracker(std::vector<K2Objects::K2TrackerBase> trackerVector);

	KINECTTOVR_API int addAndConnectTracker(K2Objects::K2TrackerBase tracker);
	KINECTTOVR_API int addAndConnectTracker(std::vector<K2Objects::K2TrackerBase> trackerVector);

	KINECTTOVR_API int connectTracker(int id);
	KINECTTOVR_API bool connectAllTrackers();
		
	KINECTTOVR_API int disconnectTracker(int id);
	KINECTTOVR_API bool disconnectAllTrackers();

	KINECTTOVR_API int updateTrackerPose(K2Objects::K2TrackerBase tracker);
	KINECTTOVR_API int updateTrackerData(K2Objects::K2TrackerBase tracker);

	KINECTTOVR_API int updateTrackerPose(int id);
	KINECTTOVR_API int updateTrackerData(int id);




	KINECTTOVR_API int setTrackerAddedCallback(function_callback callback);
	KINECTTOVR_API int setPoseUpdatedCallback(function_callback callback);
	KINECTTOVR_API int setDataUpdatedCallback(function_callback callback);

	KINECTTOVR_API void startListeningServer();
}

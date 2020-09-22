#include "pch.h"
#include "KinectToVR_API.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <utility>
#include <chrono>
#include <thread>

namespace K2API {

	int addTracker(K2Objects::K2TrackerBase tracker) {
		try {
			tracker.id = trackerList.size();
			trackerList.push_back(tracker);
			sendMessage("ADDTRACKER", tracker);
		}
		catch (std::exception e) {
			return -1;
		}
		return tracker.id;
	}
	int addTracker(std::vector<K2Objects::K2TrackerBase> trackerVector) {
		try {
			for (K2Objects::K2TrackerBase tracker : trackerVector) {
				tracker.id = trackerVector.size();
				addTracker(tracker);
			}
			sendMessage("ADDTRACKERVECTOR", trackerVector);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}

	int addAndConnectTracker(K2Objects::K2TrackerBase tracker) {
		try {
			addTracker(tracker);
			connectTracker(tracker.id);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}
	int addAndConnectTracker(std::vector<K2Objects::K2TrackerBase> trackerVector) {
		try {
			addTracker(trackerVector);
			for (K2Objects::K2TrackerBase tracker : trackerVector)
				connectTracker(tracker.id);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}

	int connectTracker(int id) {
		try {
			trackerList.at(id + 1).isActive = true;
			updateTrackerData(id);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}
	bool connectAllTrackers() {
		try {
			for (K2Objects::K2TrackerBase tracker : trackerList)
				connectTracker(tracker.id);
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

	int disconnectTracker(int id) {
		try {
			trackerList.at(id + 1).isActive = false;
			updateTrackerData(id);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}
	bool disconnectAllTrackers() {
		try {
			for (K2Objects::K2TrackerBase tracker : trackerList)
				disconnectTracker(tracker.id);
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

	int updateTrackerPose(K2Objects::K2TrackerBase tracker) {
		try {
			sendMessage("UPDATEPOSE", tracker);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}
	int updateTrackerData(K2Objects::K2TrackerBase tracker) {
		try {
			sendMessage("UPDATEDATA", tracker);
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}

	int updateTrackerPose(int id) {
		try {
			sendMessage("UPDATEPOSE", trackerList.at(id - 1));
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}
	int updateTrackerData(int id) {
		try {
			sendMessage("UPDATEDATA", trackerList.at(id - 1));
		}
		catch (std::exception e) {
			return -1;
		}
		return trackerList.size() - 1;
	}

	int setTrackerAddedCallback(function_callback callback) {
		try {
			trackerAddedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}
	int setPoseUpdatedCallback(function_callback callback) {
		try {
			poseUpdatedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}
	int setDataUpdatedCallback(function_callback callback) {
		try {
			dataUpdatedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}

	void startListeningServer() {
		std::thread([]() {
			using clock = std::chrono::steady_clock;
			auto next_frame = clock::now();

			while (true) {
				next_frame += std::chrono::milliseconds(1000 /
					(serverFramerate != 0.f ? serverFramerate : 30));

				try {
					HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\K2API_Pipe", PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND,
						/*PIPE_WAIT*/ PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 1024, 1024, 120 * 1000, NULL);

					char data[1024];
					DWORD numRead;

					ConnectNamedPipe(pipe, NULL);
					ReadFile(pipe, data, 1024, &numRead, NULL);

					if (numRead > 0) {
						std::string dat(data), command, object;
						dat = dat.substr(0, dat.rfind("/T/"));

						object = dat.substr(0, dat.rfind("/C/"));
						command = dat.substr(dat.find("/C/") + 1);

						std::istringstream ifs(dat);
						boost::archive::text_iarchive ia(ifs);

						if (command == "ADDTRACKER") {
							K2Objects::K2TrackerBase tracker;
							ia >> tracker;
							trackerAddedCallback(tracker);
						}
						if (command == "ADDTRACKERVECTOR") {
							std::vector<K2Objects::K2TrackerBase> trackerVector;
							ia >> trackerVector;
							for (K2Objects::K2TrackerBase tracker : trackerVector)
								trackerAddedCallback(tracker);
						}
						if (command == "UPDATEPOSE") {
							K2Objects::K2TrackerBase tracker;
							ia >> tracker;
							poseUpdatedCallback(tracker);
						}
						if (command == "UPDATEDATA") {
							K2Objects::K2TrackerBase tracker;
							ia >> tracker;
							dataUpdatedCallback(tracker);
						}
					}
					CloseHandle(pipe);
				}
				catch (std::exception e) {}

				std::this_thread::sleep_until(next_frame);
			}
			}).detach();
	}

	void setServerFramerate(int fps) {
		serverFramerate = fps >= 50 && fps <= 140 ? fps : 30;
	}
	void sendMessage(std::string command, K2Objects::K2TrackerBase tracker) {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << tracker;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)
		{
			throw 0;
		}

		std::string message = ofs.str() + "/C/" + command + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
	void sendMessage(std::string command, std::vector<K2Objects::K2TrackerBase> trackerVector) {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << trackerVector;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)
		{
			throw 0;
		}

		std::string message = ofs.str() + "/C/" + command + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
	void sendMessage(std::string command, int arg) {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << arg;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)
		{
			throw 0;
		}

		std::string message = ofs.str() + "/C/" + command + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
}

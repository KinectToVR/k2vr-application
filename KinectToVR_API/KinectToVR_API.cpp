#include "pch.h"
#include "KinectToVR_API.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>

#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <utility>
#include <chrono>
#include <thread>
#include <random>

namespace K2API {

	int addTracker(K2Objects::K2TrackerBase tracker) {
		try {
			uint32_t hash = getRandHash();
			sendMessage("ADDTRACKER", tracker, hash);
			tracker.id = -1;
			return getServerReply(hash);
		}
		catch (std::exception e) {}
	}
	int addTracker(std::vector<K2Objects::K2TrackerBase> trackerVector) {
		try {
			for (K2Objects::K2TrackerBase tracker : trackerVector)
				return addTracker(tracker);
		}
		catch (std::exception e) {}
	}

	int addAndConnectTracker(K2Objects::K2TrackerBase tracker) {
		try {
			addTracker(tracker);
			return connectTracker(tracker.id);
		}
		catch (std::exception e) {}
	}
	int addAndConnectTracker(std::vector<K2Objects::K2TrackerBase> trackerVector) {
		try {
			addTracker(trackerVector);
			int ret = -1;
			for (K2Objects::K2TrackerBase tracker : trackerVector)
				ret = connectTracker(tracker.id);
			return ret;
		}
		catch (std::exception e) {}
	}

	int connectTracker(int id) {
		uint32_t hash = getRandHash();
		sendMessage("CONNECTTRACKER", id, hash);
		return getServerReply(hash);
	}
	int disconnectTracker(int id) {
		uint32_t hash = getRandHash();
		sendMessage("DISCONNECTTRACKER", id, hash);
		return getServerReply(hash);
	}

	bool connectAllTrackers() {
		uint32_t hash = getRandHash();
		sendMessage("SETALLTRACKERSSTATE", 1, hash);
		return getServerReply(hash);
	}
	bool disconnectAllTrackers() {
		uint32_t hash = getRandHash();
		sendMessage("SETALLTRACKERSSTATE", 0, hash);
		return getServerReply(hash);
	}

	void updateTrackerPose(int id, K2Objects::K2PosePacket trackerPose) {
		try {
			sendMessage("UPDATEPOSE", trackerPose);
		}
		catch (std::exception e) {}
	}
	void updateTrackerData(int id, K2Objects::K2DataPacket trackerData) {
		try {
			sendMessage("UPDATEDATA", trackerData);
		}
		catch (std::exception e) {}
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
	int setPoseUpdatedCallback(function_callback_pose callback) {
		try {
			poseUpdatedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}
	int setDataUpdatedCallback(function_callback_data callback) {
		try {
			dataUpdatedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}
	int setTrackerConnectedCallback(function_callback_int callback) {
		try {
			trackerConnectedCallback = callback;
		}
		catch (std::exception e) {
			return -1;
		}
		return 0;
	}
	int setTrackerDisconnectedCallback(function_callback_int callback) {
		try {
			trackerDisconnectedCallback = callback;
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
						std::string dat(data), command, object, hash;
						int serverReply = -1;
						dat = dat.substr(0, dat.rfind("/T/"));

						object = dat.substr(0, dat.rfind("/C/"));
						command = dat.substr(dat.rfind("/C/") + 3, dat.find("/H/"));
						hash = dat.substr(dat.find("/H/") + 3);

						std::istringstream ifs(dat);
						boost::archive::text_iarchive ia(ifs);

						if (command == "ADDTRACKER") {
							K2Objects::K2TrackerBase tracker;
							ia >> tracker;
							serverReply = trackerAddedCallback(tracker);
						}
						else if (command == "CONNECTTRACKER") {
							int gotInt = -1;
							ia >> gotInt;
							serverReply = trackerConnectedCallback(gotInt);
						}
						else if (command == "DISCONNECTTRACKER") {
							int gotInt = -1;
							ia >> gotInt;
							serverReply = trackerDisconnectedCallback(gotInt);
						}
						else if (command == "CONNECTALLTRACKERS") {
							serverReply = trackerConnectedCallback(-100);
						}
						else if (command == "DISCONNECTALLTRACKERS") {
							serverReply = trackerDisconnectedCallback(-101);
						}
						else if (command == "UPDATEPOSE") {
							K2Objects::K2PosePacket trackerPose;
							ia >> trackerPose;
							poseUpdatedCallback(trackerPose);
						}
						else if (command == "UPDATEDATA") {
							K2Objects::K2DataPacket trackerData;
							ia >> trackerData;
							dataUpdatedCallback(trackerData);
						}

						if (command != "UPDATEPOSE" && command != "UPDATEDATA") {
							for (int i = 0; i < 10; i++) {
								HANDLE pipe = CreateFileA(std::string("\\\\.\\pipe\\K2API_Pipe_" + boost::lexical_cast<std::string>(hash)).c_str(),
									GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
								if (pipe == INVALID_HANDLE_VALUE) continue;

								std::string message = serverReply + "/SR/";
								DWORD numWritten;
								WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
								std::this_thread::sleep_for(std::chrono::milliseconds(2));
							}
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
	void sendMessage(std::string command, K2Objects::K2TrackerBase tracker, uint32_t hash) noexcept {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << tracker;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)return;

		std::string message = ofs.str() + "/C/" + command + "/H/" + boost::lexical_cast<std::string>(hash) + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
	void sendMessage(std::string command, K2Objects::K2PosePacket trackerPose) noexcept {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << trackerPose;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)return;

		std::string message = ofs.str() + "/C/" + command + "/H/" + boost::lexical_cast<std::string>(uint32_t(0)) + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
	void sendMessage(std::string command, K2Objects::K2DataPacket trackerData) noexcept {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << trackerData;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)return;

		std::string message = ofs.str() + "/C/" + command + "/H/" + boost::lexical_cast<std::string>(uint32_t(0)) + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}
	void sendMessage(std::string command, int arg, uint32_t hash) noexcept {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << arg;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)return;

		std::string message = ofs.str() + "/C/" + command + "/H/" + boost::lexical_cast<std::string>(hash) + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}

	int getServerReply(uint32_t hash) {
		HANDLE pipe = CreateNamedPipeA(std::string("\\\\.\\pipe\\K2API_Pipe_" + boost::lexical_cast<std::string>(hash)).c_str(), PIPE_ACCESS_INBOUND | PIPE_ACCESS_OUTBOUND,
			/*PIPE_WAIT*/ PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 1024, 1024, 120 * 1000, NULL);

		char data[1024];
		DWORD numRead;

		ConnectNamedPipe(pipe, NULL);
		ReadFile(pipe, data, 1024, &numRead, NULL);

		if (numRead > 0) {
			CloseHandle(pipe);
			std::string dat(data);
			return boost::lexical_cast<int>(dat.substr(0, dat.rfind("/SR/")));
		}
		CloseHandle(pipe);
	}
	uint32_t getRandHash() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1024, 42949672);
		return distrib(gen);
	}
}

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

	int initSocket(std::string port)
	{
		socket.connect("tcp://localhost:" + port);
	}
	
	int addTracker(K2Objects::K2TrackerBase tracker) {
		try {



			
		}
		catch (std::exception e) {}
	}
	int addTracker(std::vector<K2Objects::K2TrackerBase> trackerVector) {
		try {
			int ret = -1;
			for (K2Objects::K2TrackerBase tracker : trackerVector)
				ret = addTracker(tracker);
			return ret;
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

	std::string sendMessage_recv(const std::string data) noexcept {

		zmq::message_t message(data.size());
		std::memcpy(message.data(), data.data(), data.size());
		socket.send(message);

		// wait for reply from server
		zmq::message_t reply{};
		socket.recv(&reply);

		return reply.msg_str();
	}
	void sendMessage_norecv(const std::string data) noexcept {

		zmq::message_t message(data.size());
		std::memcpy(message.data(), data.data(), data.size());
		socket.send(message);
	}

	/*void sendMessage(std::string command, K2Objects::K2TrackerBase tracker, uint32_t hash) noexcept {
		std::ostringstream ofs;
		boost::archive::text_oarchive out(ofs);
		out << tracker;

		HANDLE pipe = CreateFileA("\\\\.\\pipe\\K2API_Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (pipe == INVALID_HANDLE_VALUE)return;

		std::string message = ofs.str() + "/C/" + command + "/H/" + boost::lexical_cast<std::string>(hash) + "/T/";
		DWORD numWritten;
		WriteFile(pipe, message.c_str(), message.length(), &numWritten, NULL);
	}*/
}

#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/assign.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Eigen/Dense>
#include "zmq.hpp"

#define K2API_GET_TIMESTAMP_NOW \
	std::chrono::time_point_cast<std::chrono::microseconds>	\
	(std::chrono::system_clock::now()).time_since_epoch().count()

#ifdef KINECTTOVR_API_EXPORTS
#define KTVR_API __declspec(dllexport)
#else
#define KTVR_API __declspec(dllimport)
#endif

namespace boost::serialization
{
	// GLM serialization
	// Just the 2 types we need
	template <typename Ar>
	void serialize(Ar& ar, glm::vec3& v, unsigned)
	{
		ar& make_nvp("x", v.x)& make_nvp("y", v.y)& make_nvp("z", v.z);
	}

	template <typename Ar>
	void serialize(Ar& ar, glm::quat& q, unsigned)
	{
		ar& make_nvp("w", q.w)& make_nvp("x", q.x)& make_nvp("y", q.y)& make_nvp("z", q.z);
	}

	// Eigen serialization
	// Just the 2 types we need
	template <class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	void serialize(Archive& ar,
		Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& t,
		const unsigned int file_version
	)
	{
		for (size_t i = 0; i < t.size(); i++)
			ar& make_nvp(("m" + std::to_string(i)).c_str(), t.data()[i]);
	}

	template <class Archive, typename _Scalar>
	void serialize(Archive& ar, Eigen::Quaternion<_Scalar>& q, unsigned)
	{
		ar& BOOST_SERIALIZATION_NVP(q.w())
			& BOOST_SERIALIZATION_NVP(q.x())
			& BOOST_SERIALIZATION_NVP(q.y())
			& BOOST_SERIALIZATION_NVP(q.z());
	}
}

namespace ktvr
{
	// Device types for K2app
	enum ITrackingDeviceType
	{
		K2_KinectV1,
		K2_KinectV2,
		K2_PSMoveService,
		K2_Other,
		K2_Unknown
	};

	// Global joint states
	enum ITrackedJointState
	{
		State_NotTracked,
		State_Inferred,
		State_Tracked
	};

	// Global Joint Types,
	// see enumeration in external/Kinect
	enum ITrackedJointType
	{
		Joint_Head,
		Joint_Neck,
		Joint_SpineShoulder,
		Joint_ShoulderLeft,
		Joint_ElbowLeft,
		Joint_WristLeft,
		Joint_HandLeft,
		Joint_HandTipLeft,
		Joint_ThumbLeft,
		Joint_ShoulderRight,
		Joint_ElbowRight,
		Joint_WristRight,
		Joint_HandRight,
		Joint_HandTipRight,
		Joint_ThumbRight,
		Joint_SpineMiddle,
		Joint_SpineWaist,
		Joint_HipLeft,
		Joint_KneeLeft,
		Joint_AnkleLeft,
		Joint_FootLeft,
		Joint_HipRight,
		Joint_KneeRight,
		Joint_AnkleRight,
		Joint_FootRight,
		Joint_Total
	};

	// OpenVR Tracker types
	enum ITrackerType
	{
		Tracker_Handed,
		Tracker_LeftFoot,
		Tracker_RightFoot,
		Tracker_LeftShoulder,
		Tracker_RightShoulder,
		Tracker_LeftElbow,
		Tracker_RightElbow,
		Tracker_LeftKnee,
		Tracker_RightKnee,
		Tracker_Waist,
		Tracker_Chest,
		Tracker_Camera,
		Tracker_Keyboard
	};

	// K2API messenging types
	enum K2MessageType {
		K2Message_Invalid, // Default
		K2Message_AddTracker, // Add
		K2Message_SetTrackerState, // State
		K2Message_SetStateAll,
		K2Message_UpdateTrackerPose, // Update
		K2Message_UpdateTrackerData,
		K2Message_DownloadTracker, // Get tracker
		K2Message_Ping // Test message
	};

	// Return messenging types
	enum K2ResponseMessageType {
		K2ResponseMessage_Invalid, // Default
		K2ResponseMessage_ID, // Just the ID
		K2ResponseMessage_Success, // State success, type only for ping
		K2ResponseMessage_Tracker // Tracker object, includes ID too!
	};

	// Return messenging types
	enum K2ResponseMessageCode {
		K2ResponseMessageCode_Exception = -10, // Excaption occured
		K2ResponseMessageCode_UnknownError = -1, // IDK
		K2ResponseMessageCode_OK = 0, // Default
		K2ResponseMessageCode_SpawnFailed = 1, // Spawn failed, exception
		K2ResponseMessageCode_AlreadyPresent = 2, // Serial already present
		K2ResponseMessageCode_BadRequest = 3, // Unknown message type, wrong ID
		K2ResponseMessageCode_ParsingError = 4, // Global parsing exception
		K2ResponseMessageCode_BadSerial = 5 // Serial was empty
	};

	// Alias for code readability
	typedef int JointTrackingState, TrackingDeviceType, MessageType, MessageCode;

	// Mapping enum to string for eliminating if-else loop
	const boost::unordered_map<ITrackerType, const char*>ITrackerType_String = boost::assign::map_list_of
	(Tracker_Handed, "vive_tracker_handed")
		(Tracker_LeftFoot, "vive_tracker_left_foot")
		(Tracker_RightFoot, "vive_tracker_right_foot")
		(Tracker_LeftShoulder, "vive_tracker_left_Shoulder")
		(Tracker_RightShoulder, "vive_tracker_right_shoulder")
		(Tracker_LeftElbow, "vive_tracker_left_elbow")
		(Tracker_RightElbow, "vive_tracker_right_elbow")
		(Tracker_LeftKnee, "vive_tracker_left_knee")
		(Tracker_RightKnee, "vive_tracker_right_knee")
		(Tracker_Waist, "vive_tracker_waist")
		(Tracker_Chest, "vive_tracker_chest")
		(Tracker_Camera, "vive_tracker_camera")
		(Tracker_Keyboard, "vive_tracker_keyboard"),

		ITrackerType_Role_String = boost::assign::map_list_of
		(Tracker_Handed, "TrackerRole_Handed")
		(Tracker_LeftFoot, "TrackerRole_LeftFoot")
		(Tracker_RightFoot, "TrackerRole_RightFoot")
		(Tracker_LeftShoulder, "TrackerRole_LeftShoulder")
		(Tracker_RightShoulder, "TrackerRole_RightShoulder")
		(Tracker_LeftElbow, "TrackerRole_LeftElbow")
		(Tracker_RightElbow, "TrackerRole_RightElbow")
		(Tracker_LeftKnee, "TrackerRole_LeftKnee")
		(Tracker_RightKnee, "TrackerRole_RightKnee")
		(Tracker_Waist, "TrackerRole_Waist")
		(Tracker_Chest, "TrackerRole_Chest")
		(Tracker_Camera, "TrackerRole_Camera")
		(Tracker_Keyboard, "TrackerRole_Keyboard");

	class K2TrackerPose
	{
	public:

		// Tracker should be centered automatically
		glm::quat orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(orientation)
				& BOOST_SERIALIZATION_NVP(position);
		}

		// Default constructors
		K2TrackerPose() = default;
		~K2TrackerPose() = default;

		// Copy constructors
		K2TrackerPose(const K2TrackerPose&) = default;
		K2TrackerPose& operator=(const K2TrackerPose&) = default;

		// Move operators
		K2TrackerPose(K2TrackerPose&&) = default;
		K2TrackerPose& operator=(K2TrackerPose&&) = default;

		// Quick constructor
		K2TrackerPose(glm::quat m_orientation, glm::vec3 m_position) :
			orientation(m_orientation), position(m_position)
		{
		}
	};

	// Assuming not autoadded by default
	// To autoadd set isActive to true
	// Data packet, as known in k2api
	class K2TrackerData
	{
	public:

		std::string serial; // Must be set manually
		int role = 0; // Handed Tracker
		bool isActive = false;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(serial)
				& BOOST_SERIALIZATION_NVP(role)
				& BOOST_SERIALIZATION_NVP(isActive);
		}

		// Default constructors
		K2TrackerData() = default;
		~K2TrackerData() = default;

		// Copy constructors
		K2TrackerData(const K2TrackerData&) = default;
		K2TrackerData& operator=(const K2TrackerData&) = default;

		// Move operators
		K2TrackerData(K2TrackerData&&) = default;
		K2TrackerData& operator=(K2TrackerData&&) = default;

		// Quick constructor
		K2TrackerData(std::string m_serial, ITrackerType m_role, bool m_isActive = false) :
			serial(std::move(m_serial)), role(m_role), isActive(m_isActive)
		{
		}
	};

	class K2PosePacket : public K2TrackerPose
	{
	public:

		double millisFromNow = 0; // Time offset after sending

		K2PosePacket() = default;
		~K2PosePacket() = default;

		// Copy constructors
		K2PosePacket(const K2PosePacket&) = default;
		K2PosePacket& operator=(const K2PosePacket&) = default;

		// Move operators
		K2PosePacket(K2PosePacket&& packet) noexcept : K2TrackerPose(packet)
		{
		}
		K2PosePacket& operator=(K2PosePacket&& packet) noexcept
		{
			K2TrackerPose::operator=(packet);
			return *this;
		}

		// Default constructor 2
		K2PosePacket(K2TrackerPose const& m_pose, int const& millis) :
			K2TrackerPose(m_pose), millisFromNow(millis)
		{
		}

		// Default constructor
		K2PosePacket(K2TrackerPose const& m_pose) :
			K2TrackerPose(m_pose)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(boost::serialization::base_object<K2TrackerPose>(*this))
				& BOOST_SERIALIZATION_NVP(millisFromNow); // Serialize via base class
		}
	};

	class K2DataPacket : public K2TrackerData
	{
	public:

		double millisFromNow = 0; // Time offset after sending

		// Default constructors
		K2DataPacket() = default;
		~K2DataPacket() = default;

		// Copy constructors
		K2DataPacket(const K2DataPacket&) = default;
		K2DataPacket& operator=(const K2DataPacket&) = default;

		// Move operators
		K2DataPacket(K2DataPacket&& packet) noexcept : K2TrackerData(packet)
		{
		}
		K2DataPacket& operator=(K2DataPacket&& packet) noexcept
		{
			K2TrackerData::operator=(packet);
			return *this;
		}

		// Default constructor 2
		K2DataPacket(K2TrackerData const& m_data, int const& millis) :
			K2TrackerData(m_data), millisFromNow(millis)
		{
		}

		// Default constructor
		K2DataPacket(K2TrackerData const& m_data) :
			K2TrackerData(m_data)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(boost::serialization::base_object<K2TrackerData>(*this))
				& BOOST_SERIALIZATION_NVP(millisFromNow); // Serialize via base class
		}
	};

	class K2TrackerBase
	{
	public:

		K2TrackerPose pose = K2TrackerPose();
		K2TrackerData data = K2TrackerData();
		int id = -1; // For error case

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(pose)
				& BOOST_SERIALIZATION_NVP(data)
				& BOOST_SERIALIZATION_NVP(id);
		}

		// Default constructors
		K2TrackerBase() = default;
		~K2TrackerBase() = default;

		// Copy constructors
		K2TrackerBase(const K2TrackerBase&) = default;
		K2TrackerBase& operator=(const K2TrackerBase&) = default;

		// Move operators
		K2TrackerBase(K2TrackerBase&&) = default;
		K2TrackerBase& operator=(K2TrackerBase&&) = default;

		K2TrackerBase(K2TrackerPose const& m_pose, K2TrackerData m_data) :
			pose(m_pose), data(std::move(m_data))
		{
		}
	};

	// Tracker: for new tracker creation
	// ID: while updating a tracker for state
	// Pose / Data: while updating a tracker
	//    :update possible via base object too
	class K2Message
	{
	public:

		// Message type, assume fail
		MessageType messageType = K2Message_Invalid;

		// Message timestamp when sent
		long long messageTimestamp = 0,
			messageManualTimestamp = 0; // This one's for mid-events

		// Since we're updating the main timestamp on creation,
		// we'd like to check other ones somehow

		// Object, parsing depends on type
		K2TrackerBase tracker_base = K2TrackerBase();
		K2PosePacket tracker_pose = K2PosePacket();
		K2DataPacket tracker_data = K2DataPacket();

		// Rest object, depends on type too
		int id = -1;
		bool state = false;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(messageType)
				& BOOST_SERIALIZATION_NVP(tracker_base)
				& BOOST_SERIALIZATION_NVP(tracker_pose)
				& BOOST_SERIALIZATION_NVP(tracker_data)
				& BOOST_SERIALIZATION_NVP(id)
				& BOOST_SERIALIZATION_NVP(state)
				& BOOST_SERIALIZATION_NVP(messageTimestamp)
				& BOOST_SERIALIZATION_NVP(messageManualTimestamp);
		}

		// Serialize as string
		std::string serializeToString() {
			std::ostringstream o;
			boost::archive::text_oarchive oa(o);
			oa << this;
			return o.str();
		}

		// Serialize from string, do not call as an overwrite
		[[nodiscard]] static K2Message parseFromString(std::string str) noexcept {
			try {
				std::istringstream i(str);
				boost::archive::text_iarchive ia(i);

				K2Message response;
				ia >> response;
				return response;
			}
			catch (boost::archive::archive_exception const& e) {}
			return K2Message();
		}

		// Default constructors
		K2Message() = default;
		~K2Message() = default;

		// Copy constructors
		K2Message(const K2Message&) = default;
		K2Message& operator=(const K2Message&) = default;

		// Move operators
		K2Message(K2Message&&) = default;
		K2Message& operator=(K2Message&&) = default;

		/*
		 * Lower constructors are made just because
		 * to have a faster way to construct a message,
		 * without caring about its type.
		 * I mean, just why not?
		 */

		 // Update the tracker's pose
		K2Message(int m_id, K2PosePacket m_pose) : id(m_id)
		{
			tracker_pose = std::move(m_pose);
			messageType = K2Message_UpdateTrackerPose;
		}

		// Update the tracker's data
		K2Message(int m_id, K2DataPacket m_data) : id(m_id)
		{
			tracker_data = std::move(m_data);
			messageType = K2Message_UpdateTrackerData;
		}

		// Add a tracker, to automatically spawn,
		// set it's state to true
		K2Message(K2TrackerBase m_tracker)
		{
			tracker_base = std::move(m_tracker);
			messageType = K2Message_AddTracker;
		}

		// Basically the upper command,
		// although written a bit different
		// It uhmmm... will let us autospawn, but at the call
		K2Message(K2TrackerBase m_tracker, bool m_state) : state(m_state)
		{
			tracker_base = std::move(m_tracker);
			messageType = K2Message_AddTracker;
		}
		
		// Set all trackers' state
		K2Message(const bool m_state) :
			state(m_state)
		{
			messageType = K2Message_SetStateAll;
		}

		// Set one tracker's state
		K2Message(const int m_id, const bool m_state) :
			id(m_id), state(m_state)
		{
			messageType = K2Message_SetTrackerState;
		}

		// Download a tracker
		K2Message(const int m_id) :
			id(m_id)
		{
			messageType = K2Message_DownloadTracker;
		}
	};

	// Tracker: for new tracker creation
	// ID: while updating a tracker for state
	// Pose / Data: while updating a tracker
	//    :update possible via base object too
	class K2ResponseMessage
	{
	public:

		// Message type, assume fail
		MessageType messageType = K2ResponseMessage_Invalid;

		// Message timestamp when sent
		long long messageTimestamp = 0,
			messageManualTimestamp = 0; // This one's for mid-events

		// Since we're updating the main timestamp on creation,
		// we'd like to check other ones somehow

		// For downloading a tracker
		K2TrackerBase tracker_base = K2TrackerBase();

		// Rest object, depends on type too
		int id = -1; // Tracker's id, assume fail
		MessageCode result = -1; // For error case
		bool success = false;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_NVP(messageType)
				& BOOST_SERIALIZATION_NVP(tracker_base)
				& BOOST_SERIALIZATION_NVP(id)
				& BOOST_SERIALIZATION_NVP(result)
				& BOOST_SERIALIZATION_NVP(success)
				& BOOST_SERIALIZATION_NVP(messageTimestamp)
				& BOOST_SERIALIZATION_NVP(messageManualTimestamp);
		}

		// Serialize as string
		std::string serializeToString() {
			std::ostringstream o;
			boost::archive::text_oarchive oa(o);
			oa << this;
			return o.str();
		}

		// Serialize from string, do not call as an overwriter
		[[nodiscard]] static K2ResponseMessage parseFromString(std::string str) noexcept {
			try {
				std::istringstream i(str);
				boost::archive::text_iarchive ia(i);

				K2ResponseMessage response;
				ia >> response;
				return response;
			}
			catch (boost::archive::archive_exception const& e) {}
			return K2ResponseMessage();
		}

		// Default constructors
		K2ResponseMessage() = default;
		~K2ResponseMessage() = default;

		// Copy constructors
		K2ResponseMessage(const K2ResponseMessage&) = default;
		K2ResponseMessage& operator=(const K2ResponseMessage&) = default;

		// Move operators
		K2ResponseMessage(K2ResponseMessage&&) = default;
		K2ResponseMessage& operator=(K2ResponseMessage&&) = default;

		/*
		 * Lower constructors are made just because
		 * to have a faster way to construct a message,
		 * without caring about its type.
		 * I mean, just why not?
		 */

		 // ID as the response
		K2ResponseMessage(const int m_id) :
			id(m_id)
		{
			messageType = K2ResponseMessage_ID;
		}

		// Success return, eg for ping or somewhat
		// Set all trackers' state
		K2ResponseMessage(const bool m_state) :
			success(m_state)
		{
			messageType = K2ResponseMessage_Success;
		}

		// Return whole tracker object: creation / download
		K2ResponseMessage(K2TrackerBase m_tracker) :
			tracker_base(std::move(m_tracker))
		{
			messageType = K2ResponseMessage_Tracker;
		}
	};

	/**
	 * \brief Dump std::string to hex character string
	 * \param s String to be converted
	 * \return Returns hex string
	 */
	KTVR_API std::string hexString(const std::string& s);

	/**
	 * \brief Dump hex std::string to ascii character string
	 * \param s String to be converted
	 * \return Returns ascii string
	 */
	KTVR_API std::string asciiString(const std::string& s);

	/// <summary>
	/// ZMQ context for sensing and receiving messages
	/// </summary>
	inline zmq::context_t context{ 1 };
	inline zmq::socket_t socket{ context, zmq::socket_type::req };

	/**
	 * \brief Connects socket object to selected port, K2 uses 7135
	 * \param port TCP port on which should be all calls created
	 * \return Returns 0 for success and -1 for failure
	 */
	KTVR_API int init_socket(int port) noexcept;

	/**
	 * \brief Disconnects socket object from port
	 * \return Returns 0 for success and -1 for failure
	 */
	KTVR_API int close_socket() noexcept;

	/**
	 * \brief Send message via ZMQ and get a server reply, there is no need to decode return
	 * \param data String which is to be sent
	 * \return Returns server's reply to the message
	 */
	KTVR_API [[nodiscard]] std::string send_message(std::string const& data) noexcept(false);

	/**
	 * \brief Send message and get a server reply
	 * \param message Message which is to be sent
	 * \return Returns server's reply to the message
	 */
	KTVR_API ktvr::K2ResponseMessage send_message(ktvr::K2Message message) noexcept(false);

	/**
	 * \brief Add tracker to driver's trackers list
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker object / id / success, overwrites object's id
	 */
	KTVR_API [[nodiscard]] ktvr::K2ResponseMessage add_tracker(ktvr::K2TrackerBase& tracker) noexcept;

	/**
	 * \brief Connect (activate/spawn) tracker in SteamVR
	 * \param id Tracker's id which is to connect
	 * \param state Tracker's state to be set
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage set_tracker_state(int id, bool state) noexcept;

	/**
	 * \brief Connect (activate/spawn) all trackers in SteamVR
	 * \param state Tracker's state to be set
	 * \return Returns success?
	 */
	KTVR_API ktvr::K2ResponseMessage set_state_all(bool state) noexcept;

	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param id Tracker's id which is to update
	 * \param tracker_pose New pose for tracker
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage update_tracker_pose(int id, ktvr::K2PosePacket const& tracker_pose) noexcept;

	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param tracker_handle Tracker for updating data
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage update_tracker_pose(ktvr::K2TrackerBase const& tracker_handle) noexcept;

	/**
	 * \brief Update tracker's data in SteamVR driver (ONLY for yet not spawned trackers)
	 * \param id Tracker's id which is to update
	 * \param tracker_data New pose for tracker
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage update_tracker_data(int id, ktvr::K2DataPacket const& tracker_data) noexcept;

	/**
	 * \brief Update tracker's data in SteamVR driver
	 * \param tracker_handle Tracker for updating data
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage update_tracker_data(ktvr::K2TrackerBase const& tracker_handle) noexcept;

	/**
	 * \brief Update tracker in SteamVR driver
	 * \param tracker Tracker base to be updated from
	 * \return Returns tracker id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage update_tracker(ktvr::K2TrackerBase const& tracker) noexcept;

	/**
	 * \brief Grab all possible data from existing tracker
	 * \param tracker_id Tracker id for download
	 * \return Returns tracker object / id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage download_tracker(int const& tracker_id) noexcept;

	/**
	 * \brief Grab all possible data from existing tracker
	 * \param tracker Tracker base id is to be grabbed from
	 * \return Returns tracker object / id / success?
	 */
	KTVR_API ktvr::K2ResponseMessage download_tracker(ktvr::K2TrackerBase const& tracker) noexcept;

	/**
	 * \brief Test connection with the server
	 * \return Returns send_time / total_time / success?
	 */
	KTVR_API std::tuple<ktvr::K2ResponseMessage, long long, long long> test_connection() noexcept;
}


#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <span>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/optional.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

#include <Eigen/Dense>

/*
 * Default IPC defines are:
 *
 * \\\\.\\pipe\\k2api_to_pipe
 * \\\\.\\pipe\\k2api_from_pipe
 *
 * Global\\k2api_to_sem
 * Global\\k2api_from_sem
 * Global\\k2api_start_sem
 */

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
	// Eigen serialization
	template <class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	void serialize(Archive& ar,
	               Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& t,
	               const unsigned int file_version
	)
	{
		for (size_t i = 0; i < t.size(); i++)
			ar & make_nvp(("m" + std::to_string(i)).c_str(), t.data()[i]);
	}

	template <class Archive, typename _Scalar>
	void serialize(Archive& ar, Eigen::Quaternion<_Scalar>& q, unsigned)
	{
		ar & BOOST_SERIALIZATION_NVP(q.w())
			& BOOST_SERIALIZATION_NVP(q.x())
			& BOOST_SERIALIZATION_NVP(q.y())
			& BOOST_SERIALIZATION_NVP(q.z());
	}

	template <class Archive, typename _Scalar>
	void serialize(Archive& ar, Eigen::Vector3<_Scalar>& v, unsigned)
	{
		ar & BOOST_SERIALIZATION_NVP(v.x())
			& BOOST_SERIALIZATION_NVP(v.y())
			& BOOST_SERIALIZATION_NVP(v.z());
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

	// K2API messaging types
	enum K2MessageType
	{
		K2Message_Invalid,
		// Default
		K2Message_AddTracker,
		// Add
		K2Message_SetTrackerState,
		K2Message_SetStateAll,
		// State
		K2Message_UpdateTrackerPose,
		K2Message_UpdateTrackerData,
		// Update
		K2Message_DownloadTracker,
		// Get tracker
		K2Message_RefreshTracker,
		// Refresh tracker pose
		K2Message_RequestRestart,
		// Request a restart
		K2Message_Ping
		// Test message
	};

	// Return messaging types
	enum K2ResponseMessageType
	{
		K2ResponseMessage_Invalid,
		// Default
		K2ResponseMessage_ID,
		// Just the ID
		K2ResponseMessage_Success,
		// State success, type only for ping
		K2ResponseMessage_Tracker // Tracker object, includes ID too!
	};

	// Return messenging types
	enum K2ResponseMessageCode
	{
		K2ResponseMessageCode_Exception = -10,
		// Excaption occured
		K2ResponseMessageCode_UnknownError = -1,
		// IDK
		K2ResponseMessageCode_Invalid = 0,
		// Default Invalid
		K2ResponseMessageCode_OK = 1,
		// Default OK
		K2ResponseMessageCode_SpawnFailed = 2,
		// Spawn failed, exception
		K2ResponseMessageCode_AlreadyPresent = 3,
		// Serial already present
		K2ResponseMessageCode_BadRequest = 4,
		// Unknown message type, wrong ID
		K2ResponseMessageCode_ParsingError = 5,
		// Global parsing exception
		K2ResponseMessageCode_BadSerial = 6 // Serial was empty
	};

	// Alias for code readability
	typedef int JointTrackingState, TrackingDeviceType, MessageType, MessageCode;

	// Mapping enum to string for eliminating if-else loop
	const boost::unordered_map<ITrackerType, const char*>
		ITrackerType_String = boost::assign::map_list_of
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

	// Check Eigen quaternions
	template <typename _Scalar>
	Eigen::Quaternion<_Scalar> quaternion_normal(const Eigen::Quaternion<_Scalar>& q)
	{
		return Eigen::Quaternion<_Scalar>(
			boost::math::isnormal(q.w()) ? q.w() : 1.f,
			boost::math::isnormal(q.x()) ? q.x() : 0.f,
			boost::math::isnormal(q.y()) ? q.y() : 0.f,
			boost::math::isnormal(q.z()) ? q.z() : 0.f
		);
	}

	// Check Eigen Vectors
	template <typename _Scalar>
	Eigen::Vector3<_Scalar> vector3_normal(const Eigen::Vector3<_Scalar>& v)
	{
		return Eigen::Vector3<_Scalar>(
			boost::math::isnormal(v.x()) ? v.x() : 0.f,
			boost::math::isnormal(v.y()) ? v.y() : 0.f,
			boost::math::isnormal(v.z()) ? v.z() : 0.f
		);
	}
	
	// Get file location in AppData
	inline std::string GetK2AppDataFileDir(const std::string& relativeFilePath)
	{
		CreateDirectoryA(std::string(std::string(std::getenv("APPDATA")) + "\\KinectToVR\\").c_str(),
			nullptr);
		return std::string(std::getenv("APPDATA")) + "\\KinectToVR\\" + relativeFilePath;
	}

	// Get file location in AppData
	inline std::string GetK2AppDataLogFileDir(const std::string& relativeFilePath)
	{
		CreateDirectoryA(std::string(std::string(std::getenv("APPDATA")) + "\\KinectToVR\\logs\\").c_str(),
			nullptr);
		return std::string(std::getenv("APPDATA")) + "\\KinectToVR\\logs\\" + relativeFilePath;
	}

	class K2Vector3
	{
	public:
		float x = 0.f, y = 0.f, z = 0.f;

		// Default constructors
		K2Vector3() = default;
		~K2Vector3() = default;

		// Copy constructors
		K2Vector3(const K2Vector3&) = default;
		K2Vector3& operator=(const K2Vector3&) = default;

		// Move operators
		K2Vector3(K2Vector3&&) = default;
		K2Vector3& operator=(K2Vector3&&) = default;

		K2Vector3(float _x, float _y, float _z) :
			x(_x), y(_y), z(_z)
		{
		}

		K2Vector3(Eigen::Vector3f v) :
			x(v.x()), y(v.y()), z(v.z())
		{
		}

		// Conversion
		operator Eigen::Vector3f() const
		{
			return Eigen::Vector3f(x, y, z);
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (!boost::math::isnormal(x))x = 0.f;
			if (!boost::math::isnormal(y))y = 0.f;
			if (!boost::math::isnormal(z))z = 0.f;

			ar & BOOST_SERIALIZATION_NVP(x) & BOOST_SERIALIZATION_NVP(y) & BOOST_SERIALIZATION_NVP(z);
		}
	};

	class K2Quaternion
	{
	public:
		float w = 0.f, x = 0.f, y = 0.f, z = 0.f;

		// Default constructors
		K2Quaternion() = default;
		~K2Quaternion() = default;

		// Copy constructors
		K2Quaternion(const K2Quaternion&) = default;
		K2Quaternion& operator=(const K2Quaternion&) = default;

		// Move operators
		K2Quaternion(K2Quaternion&&) = default;
		K2Quaternion& operator=(K2Quaternion&&) = default;

		K2Quaternion(float _w, float _x, float _y, float _z) :
			w(_w), x(_x), y(_y), z(_z)
		{
		}

		K2Quaternion(Eigen::Quaternionf q) :
			w(q.w()), x(q.x()), y(q.y()), z(q.z())
		{
		}

		// Conversion
		operator Eigen::Quaternionf() const
		{
			return Eigen::Quaternionf(w, x, y, z);
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (!boost::math::isnormal(w))w = 1.f;
			if (!boost::math::isnormal(x))x = 0.f;
			if (!boost::math::isnormal(y))y = 0.f;
			if (!boost::math::isnormal(z))z = 0.f;

			ar & BOOST_SERIALIZATION_NVP(w) & BOOST_SERIALIZATION_NVP(x) & BOOST_SERIALIZATION_NVP(y) &
				BOOST_SERIALIZATION_NVP(z);
		}
	};

	class K2TrackerPose
	{
	public:
		// Tracker should be centered automatically
		Eigen::Quaternionf orientation = Eigen::Quaternionf(1.f, 0.f, 0.f, 0.f);
		Eigen::Vector3f position = Eigen::Vector3f(0.f, 0.f, 0.f);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(orientation) & BOOST_SERIALIZATION_NVP(position);
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
		K2TrackerPose(Eigen::Quaternionf m_orientation, Eigen::Vector3f m_position) :
			orientation(std::move(m_orientation)), position(std::move(m_position))
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
			ar & BOOST_SERIALIZATION_NVP(serial) & BOOST_SERIALIZATION_NVP(role) & BOOST_SERIALIZATION_NVP(isActive);
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
		K2PosePacket(const K2TrackerPose& m_pose, const int& millis) :
			K2TrackerPose(m_pose), millisFromNow(millis)
		{
		}

		// Default constructor
		K2PosePacket(const K2TrackerPose& m_pose) :
			K2TrackerPose(m_pose)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(orientation) & BOOST_SERIALIZATION_NVP(position) &
				BOOST_SERIALIZATION_NVP(millisFromNow); // Serialize
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
		K2DataPacket(const K2TrackerData& m_data, const int& millis) :
			K2TrackerData(m_data), millisFromNow(millis)
		{
		}

		// Default constructor
		K2DataPacket(const K2TrackerData& m_data) :
			K2TrackerData(m_data)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(serial) & BOOST_SERIALIZATION_NVP(role) & BOOST_SERIALIZATION_NVP(isActive)
				& BOOST_SERIALIZATION_NVP(millisFromNow); // Serialize
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
			ar & BOOST_SERIALIZATION_NVP(pose) & BOOST_SERIALIZATION_NVP(data) & BOOST_SERIALIZATION_NVP(id);
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

		K2TrackerBase(const K2TrackerPose& m_pose, K2TrackerData m_data) :
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
		boost::optional<K2TrackerBase> tracker_base;
		boost::optional<K2PosePacket> tracker_pose;
		K2DataPacket tracker_data;

		// Rest object, depends on type too
		int id = -1;
		bool state = false, want_reply = true;
		std::string message_string; // Placeholder for anything

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(messageType)
				& BOOST_SERIALIZATION_NVP(tracker_base)
				& BOOST_SERIALIZATION_NVP(tracker_pose)
				& BOOST_SERIALIZATION_NVP(tracker_data)
				& BOOST_SERIALIZATION_NVP(message_string)
				& BOOST_SERIALIZATION_NVP(id)
				& BOOST_SERIALIZATION_NVP(state)
				& BOOST_SERIALIZATION_NVP(want_reply)
				& BOOST_SERIALIZATION_NVP(messageTimestamp)
				& BOOST_SERIALIZATION_NVP(messageManualTimestamp);
		}

		// Serialize as string
		std::string serializeToString()
		{
			std::ostringstream o;
			boost::archive::text_oarchive oa(o);
			oa << *this;
			return o.str();
		}

		// Serialize from string, do not call as an overwrite
		[[nodiscard]] static K2Message parseFromString(const std::string& str) noexcept
		{
			try
			{
				std::istringstream i(str);
				boost::archive::text_iarchive ia(i);

				K2Message response;
				ia >> response;
				return response;
			}
			catch (const boost::archive::archive_exception& e)
			{
				return K2Message();
			}
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
		 * Lower constructors are made just
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
			ar & BOOST_SERIALIZATION_NVP(messageType)
				& BOOST_SERIALIZATION_NVP(tracker_base)
				& BOOST_SERIALIZATION_NVP(id)
				& BOOST_SERIALIZATION_NVP(result)
				& BOOST_SERIALIZATION_NVP(success)
				& BOOST_SERIALIZATION_NVP(messageTimestamp)
				& BOOST_SERIALIZATION_NVP(messageManualTimestamp);
		}

		// Serialize as string
		std::string serializeToString()
		{
			std::ostringstream o;
			boost::archive::text_oarchive oa(o);
			oa << *this;
			return o.str();
		}

		// Serialize from string, do not call as an overwriter
		[[nodiscard]] static K2ResponseMessage parseFromString(const std::string& str) noexcept
		{
			try
			{
				std::istringstream i(str);
				boost::archive::text_iarchive ia(i);

				K2ResponseMessage response;
				ia >> response;
				return response;
			}
			catch (const boost::archive::archive_exception& e)
			{
			}
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
	/// K2API Semaphore handles for WINAPI calls
	/// </summary>
	inline HANDLE k2api_to_Semaphore,
	              k2api_from_Semaphore,
	              k2api_start_Semaphore;

	/// <summary>
	/// K2API's last error string, check for empty
	/// </summary>
	inline std::string k2api_last_error;

	/// <summary>
	/// Get K2API's last error string
	/// </summary>
	inline std::string GetLastError() { return k2api_last_error; }

	/// <summary>
	/// K2API Pipe handle addresses for WINAPI calls
	/// </summary>
	inline std::string
		k2api_to_pipe_address,
		k2api_from_pipe_address;

	/**
	 * \brief Connects socket object to selected port, K2 uses 7135
	 * \return Returns 0 for success and -1 for failure
	 */
	KTVR_API int init_k2api(
		const std::string& k2_to_pipe = "\\\\.\\pipe\\k2api_to_pipe",
		const std::string& k2_from_pipe = "\\\\.\\pipe\\k2api_from_pipe",
		const std::string& k2_to_sem = "Global\\k2api_to_sem",
		const std::string& k2_from_sem = "Global\\k2api_from_sem",
		const std::string& k2_start_sem = "Global\\k2api_start_sem") noexcept;

	/**
	 * \brief Disconnects socket object from port
	 * \return Returns 0 for success and -1 for failure
	 */
	KTVR_API int close_k2api() noexcept;

	/**
	 * \brief Send message and get a server reply, there is no need to decode return
	 * \param data String which is to be sent
	 * \param want_reply Check if the client wants a reply
	 * \return Returns server's reply to the message
	 */
	KTVR_API std::string send_message(const std::string& data, bool want_reply = true) noexcept(false);

	/**
	 * \brief Send message and get a server reply
	 * \param message Message which is to be sent
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns server's reply to the message
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	send_message(K2Message message) noexcept(false)
	{
		// Add timestamp
		message.messageTimestamp = K2API_GET_TIMESTAMP_NOW;
		message.want_reply = want_reply;

		// Serialize to string
		std::ostringstream o;
		boost::archive::text_oarchive oa(o);
		oa << message;

		// Send the message
		// Deserialize then
		if constexpr (want_reply)
		{
			// Compose the response
			K2ResponseMessage response;
			auto reply = send_message(o.str(), want_reply);

			std::istringstream i(reply);
			boost::archive::text_iarchive ia(i);
			ia >> response;

			// Deserialize reply and return
			return std::move(response);
		}
		else
		{
			// Probably void
			send_message(o.str(), want_reply);
			return std::monostate();
		}
	}

	/**
	 * \brief Add tracker to driver's trackers list
	 * \param tracker Tracker base that should be used for device creation
	 * \return Returns new tracker object / id / success, overwrites object's id
	 */
	KTVR_API [[nodiscard]] K2ResponseMessage add_tracker(K2TrackerBase& tracker) noexcept;

	/**
	 * \brief Connect (activate/spawn) tracker in SteamVR
	 * \param id Tracker's id which is to connect
	 * \param state Tracker's state to be set
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	set_tracker_state(int id, bool state) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message<want_reply>(
				K2Message(id, state));
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Connect (activate/spawn) all trackers in SteamVR
	 * \param state Tracker's state to be set
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	set_state_all(bool state) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message
			return send_message<want_reply>(
				K2Message(state));
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param id Tracker's id which is to update
	 * \param tracker_pose New pose for tracker
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	update_tracker_pose(int id, const K2PosePacket& tracker_pose) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message<want_reply>(
				K2Message(id, tracker_pose));
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}


	/**
	 * \brief Update tracker's pose in SteamVR driver
	 * \param tracker_handle Tracker for updating data
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	update_tracker_pose(const K2TrackerBase& tracker_handle) noexcept
	{
		try
		{
			// Send the message and return
			return update_tracker_pose<want_reply>(tracker_handle.id, tracker_handle.pose);
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Update tracker's data in SteamVR driver (ONLY for yet not spawned trackers)
	 * \param id Tracker's id which is to update
	 * \param tracker_data New pose for tracker
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	update_tracker_data(int id, const K2DataPacket& tracker_data) noexcept
	{
		try
		{
			// Send and grab the response
			// Thanks to our constructors,
			// message will set all
			// Send the message and return
			return send_message<want_reply>(
				K2Message(id, tracker_data));
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Update tracker's data in SteamVR driver
	 * \param tracker_handle Tracker for updating data
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	update_tracker_data(const K2TrackerBase& tracker_handle) noexcept
	{
		try
		{
			// Send the message and return
			return update_tracker_data<want_reply>(tracker_handle.id, tracker_handle.data);
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Update tracker in SteamVR driver
	 * \param tracker Tracker base to be updated from
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	update_tracker(const K2TrackerBase& tracker) noexcept
	{
		try
		{
			// Send the message and return
			update_tracker_pose<want_reply>(tracker.id, tracker.pose);

			// Data is more important then return data
			return update_tracker_data<want_reply>(tracker.id, tracker.data);
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Update tracker's pose in SteamVR driver with already existing values
	 * \param tracker_id Tracker for updating data
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	refresh_tracker_pose(const int& tracker_id) noexcept
	{
		try
		{
			// Send and grab the response
			auto message = K2Message();
			message.id = tracker_id;
			message.messageType = K2Message_RefreshTracker;

			// Send the message and return
			return send_message<want_reply>(message);
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Request OpenVR to restart with a message
	 * \param reason Reason for the restart
	 * \argument want_reply Check if the client wants a reply
	 * \return Returns tracker id / success?
	 */
	template <bool want_reply = true>
	typename std::conditional<want_reply, K2ResponseMessage, std::monostate>::type
	request_vr_restart(const std::string& reason) noexcept
	{
		try
		{
			// Send and grab the response
			auto message = K2Message();
			message.message_string = std::move(reason);
			message.messageType = K2Message_RequestRestart;

			// Send the message and return
			return send_message<want_reply>(message);
		}
		catch (const std::exception& e)
		{
			if constexpr (want_reply) return K2ResponseMessage(); // Success is set to false by default
			else return std::monostate();
		}
	}

	/**
	 * \brief Grab all possible data from existing tracker
	 * \param tracker_id Tracker id for download
	 * \return Returns tracker object / id / success?
	 */
	KTVR_API K2ResponseMessage download_tracker(const int& tracker_id) noexcept;

	/**
	 * \brief Grab all possible data from existing tracker
	 * \param tracker_serial Tracker id for download
	 * \return Returns tracker object / id / success?
	 */
	KTVR_API K2ResponseMessage download_tracker(const std::string& tracker_serial) noexcept;

	/**
	 * \brief Grab all possible data from existing tracker
	 * \param tracker Tracker base id is to be grabbed from
	 * \return Returns tracker object / id / success?
	 */
	KTVR_API K2ResponseMessage download_tracker(const K2TrackerBase& tracker) noexcept;

	/**
	 * \brief Test connection with the server
	 * \return Returns send_time / total_time / success?
	 */
	KTVR_API std::tuple<K2ResponseMessage, long long, long long> test_connection() noexcept;
}

#pragma once
#include <boost/serialization/array.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

// Alias for code readability
typedef int JointTrackingState, TrackingDeviceType;

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

// GLM serialization
// Just the 2 types we need
namespace boost::serialization
{
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
}

namespace K2Objects
{
	class K2TrackerPose
	{
	public:

		// Tracker should be centered automatically
		glm::quat orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& orientation& position;
		}

		K2TrackerPose() = default;
		~K2TrackerPose() = default;

		// Quick constructor
		K2TrackerPose(glm::quat m_orientation, glm::vec3 m_position) :
			orientation(m_orientation), position(m_position)
		{
		}
	};

	class K2TrackerData
	{
	public:

		std::string serial; // Must be set manually
		int role = 0; // Handed Tracker
		bool isActive = false;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& serial& role& isActive;
		}

		K2TrackerData() = default;
		~K2TrackerData() = default;

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

		K2PosePacket(K2TrackerPose m_pose) : K2TrackerPose(m_pose)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<K2TrackerPose>(*this)
				& millisFromNow; // Serialize via base class
		}
	};

	class K2DataPacket : public K2TrackerData
	{
	public:

		double millisFromNow = 0; // Time offset after sending

		K2DataPacket() = default;
		~K2DataPacket() = default;

		K2DataPacket(K2TrackerData m_data) : K2TrackerData(m_data)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<K2TrackerData>(*this)
				& millisFromNow; // Serialize via base class
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
			ar& pose& data& id;
		}

		K2TrackerBase() = default;
		~K2TrackerBase() = default;

		K2TrackerBase(K2TrackerPose m_pose, K2TrackerData m_data) :
			pose(m_pose), data(m_data)
		{
		}
	};
}

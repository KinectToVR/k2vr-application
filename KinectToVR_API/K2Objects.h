#pragma once
#include <boost/serialization/array.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// GLM serialization
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

		glm::quat orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& orientation& position;
		}

		K2TrackerPose() = default;
		~K2TrackerPose() = default;

		K2TrackerPose(glm::quat m_orientation, glm::vec3 m_position) :
			orientation(m_orientation), position(m_position)
		{
		}
	};

	class K2TrackerData
	{
	public:

		std::string serial = "";
		bool isActive = false;
		int role = 0; // Handed Tracker

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& serial& role& isActive;
		}

		K2TrackerData() = default;
		~K2TrackerData() = default;

		K2TrackerData(std::string m_serial, int m_role, bool m_isActive = false) :
			serial(std::move(m_serial)), role(std::move(m_role)), isActive(m_isActive)
		{
		}
	};

	class K2PosePacket : public K2TrackerPose
	{
	public:

		double millisFromNow = 0;

		K2PosePacket() = default;
		~K2PosePacket() = default;

		K2PosePacket(K2TrackerPose m_pose) : K2TrackerPose(m_pose)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<K2TrackerPose>(*this)
				& millisFromNow;
		}
	};

	class K2DataPacket : public K2TrackerData
	{
	public:

		double millisFromNow = 0;

		K2DataPacket() = default;
		~K2DataPacket() = default;

		K2DataPacket(K2TrackerData m_data) : K2TrackerData(m_data)
		{
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<K2TrackerData>(*this)
				& millisFromNow;
		}
	};

	class K2TrackerBase
	{
	public:

		K2TrackerPose pose = K2TrackerPose();
		K2TrackerData data = K2TrackerData();
		int id = -1;

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

#pragma once
#include <boost_serialization_eigen.h>

namespace K2Objects
{
	class K2TrackerPose
	{
	public:

		Eigen::Matrix3f orientation;
		Eigen::Vector3f position;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & orientation & position;
		}

		K2TrackerPose() = default;
		~K2TrackerPose() = default;

		K2TrackerPose(Eigen::Matrix3f m_orientation, Eigen::Vector3f m_position) :
			orientation(std::move(m_orientation)), position(std::move(m_position))
		{
		}
	};

	class K2TrackerData
	{
	public:

		std::string serial, role;
		bool isActive = false;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & serial & role & isActive;
		}

		K2TrackerData() = default;
		~K2TrackerData() = default;

		K2TrackerData(std::string m_serial, std::string m_role, bool m_isActive = false) :
			serial(std::move(m_serial)), role(std::move(m_role)), isActive(m_isActive)
		{
		}
	};

	class K2PosePacket : public K2TrackerPose
	{
	public:

		double millisFromNow = 0;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<K2TrackerPose>(*this)
				& millisFromNow;
		}
	};

	class K2DataPacket : public K2TrackerData
	{
	public:

		double millisFromNow = 0;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<K2TrackerData>(*this)
				& millisFromNow;
		}
	};

	class K2TrackerBase
	{
	public:

		K2TrackerPose pose;
		K2TrackerData data;
		int id = -1;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & pose & data & id;
		}

		K2TrackerBase() = default;
		~K2TrackerBase() = default;

		K2TrackerBase(K2TrackerPose m_pose, K2TrackerData m_data) :
			pose(m_pose), data(m_data)
		{
		}
	};
}

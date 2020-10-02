#pragma once
#include <Eigen/Dense>
#include <KinectToVR/boost_serialization_eigen.h>

namespace K2Objects {

	class K2TrackerPose {
	public:

		Eigen::Matrix3f orientation;
		Eigen::Vector3f position;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& orientation& position;
		}
	};

	class K2TrackerData {
	public:

		std::string serial, role;
		bool isActive = false;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& serial& role& isActive;
		}
	};

	class K2PosePacket : public K2TrackerPose {
	public:

		double millisFromNow = 0;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& boost::serialization::base_object<K2TrackerPose>(*this)
				& millisFromNow;
		}
	};

	class K2DataPacket : public K2TrackerData {
	public:

		double millisFromNow = 0;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& boost::serialization::base_object<K2TrackerData>(*this)
				& millisFromNow;
		}
	};

	class K2TrackerBase {
	public:

		K2TrackerPose m_pose;
		K2TrackerData m_data;
		int id = -1;

		K2TrackerBase(K2TrackerData data) {
			m_data = data;
		}

		K2TrackerBase() { }
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& m_pose& m_data& id;
		}
	};

}

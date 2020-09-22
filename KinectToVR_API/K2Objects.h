#pragma once
#include <Eigen/Dense>
#include <KinectToVR/boost_serialization_eigen.h>

namespace K2Objects {

	class K2TrackerBase {
	public:
		Eigen::Matrix3f orientation;
		Eigen::Vector3f position;
		std::string serial, role;
		bool isActive = false;
		int id = -1;

		K2TrackerBase(std::string m_serial, std::string m_role) {
			serial = m_serial;
			role = m_role;
		}

		K2TrackerBase() { }
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& orientation& position& serial& role& isActive;
		}
	};





}

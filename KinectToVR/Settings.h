#pragma once
#include <fstream>
#include <iostream>
#include <boost_serialization_eigen.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <Eigen/Dense>
#include <array>
#include <runtimeConfig.h>

typedef int waistRotationOption,
feetRotationOption, trackingFilterOption;

/* feet orientation option enumeration for getting it easier */
enum feetRotationFilterOptions {
	k_EnableFeetOrientationFilter,
	k_EnableFeetOrientationFilter_UseHeadOrientation,
	k_DisableFeetOrientationFilter
};

/* waist orientation option enumeration for getting it easier */
enum waistRotationFilterOptions {
	k_EnableWaistOrientationFilter,
	k_EnableWaistOrientationFilter_UseHeadOrientation,
	k_DisableWaistOrientationFilter
};

/* tracking filter option enumeration for getting it easier */
enum trackingFilterOptions {
	k_EnableTrackingFilter_LERP,
	k_EnableTrackingFilter_LowPass,
	k_EnableTrackingFilter_Kalman,
	k_DisableTrackingFilter
};

/* Class settings with all defines, comments in cpp file */
class Settings
{

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version);

public:
	void saveSettings(), readSettings();
	waistRotationOption waistOrientationTrackingOption = waistRotationFilterOptions::k_EnableWaistOrientationFilter;
	feetRotationOption feetOrientationTrackingOption = feetRotationFilterOptions::k_EnableFeetOrientationFilter;
	trackingFilterOption positionalTrackingFilterOption = trackingFilterOptions::k_EnableTrackingFilter_LERP;

	glm::vec3 playspaceOrigin = glm::vec3{ 0,0,0 };
	std::array<glm::quat, 3> glOrientationOffsets{ {glm::quat(), glm::quat(), glm::quat()} };

	std::array<Eigen::Vector3f, 3> positionalOffsets{ {Eigen::Vector3f(), Eigen::Vector3f(), Eigen::Vector3f()} };
	std::array<Eigen::Quaternionf, 3> orientationOffsets{ {Eigen::Quaternionf(), Eigen::Quaternionf(), Eigen::Quaternionf()} };

	Eigen::Matrix<float, 3, 3> rotationMatrix = Eigen::Matrix3f::Zero();
	Eigen::Matrix<float, 3, 1> translationVector = Eigen::Vector3f::Zero();

	double radPlayspaceOffset = 0.f;
	bool flipSkeleton = true;

};

extern Settings kinectSettings;
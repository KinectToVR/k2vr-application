#pragma once
#include <fstream>
#include <iostream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <Eigen/Dense>

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

	glm::vec3 playspaceOrigin = glm::vec3{ 0,0,0 },
		waistOffset = glm::vec3{ 0,0,0 },
		footRightOffset = glm::vec3{ 0,0,0 },
		footLeftOffset = glm::vec3{ 0,0,0 };
	glm::quat waistRotOffset = glm::quat{ 1,0,0,0 },
		footRightRotOffset = glm::quat{ 1,0,0,0 },
		footLeftRotOffset = glm::quat{ 1,0,0,0 };

	Eigen::Matrix<float, 3, 3> rotationMatrix = Eigen::Matrix3f::Zero();
	Eigen::Matrix<float, 3, 1> translationVector = Eigen::Vector3f::Zero();

	double radPlayspaceOffset = 0.f;

};

extern Settings kinectSettings;
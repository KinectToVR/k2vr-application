#pragma once
#include <fstream>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <Eigen/Dense>
#include <array>
#include <runtimeConfig.h>
#include <K2STracker.h>
#include "Logging.h"
#include <KinectToVR_API.h>

typedef int waistRotationOption,
            feetRotationOption, trackingFilterOption;

/* feet orientation option enumeration for getting it easier */
enum feetRotationFilterOptions
{
	k_EnableFeetOrientationFilter,
	k_EnableFeetOrientationFilter_UseHeadOrientation,
	k_DisableFeetOrientationFilter
};

/* waist orientation option enumeration for getting it easier */
enum waistRotationFilterOptions
{
	k_EnableWaistOrientationFilter,
	k_EnableWaistOrientationFilter_UseHeadOrientation,
	k_DisableWaistOrientationFilter
};

/* tracking filter option enumeration for getting it easier */
enum trackingFilterOptions
{
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
	template <class Archive>
	void serialize(Archive& archive, unsigned int version);

public:
	static void saveSettings(), readSettings();
	waistRotationOption waistOrientationTrackingOption = k_EnableWaistOrientationFilter;
	feetRotationOption feetOrientationTrackingOption = k_EnableFeetOrientationFilter;
	trackingFilterOption globalPositionTrackingFilterOption = k_EnableTrackingFilter_LERP;

	Eigen::Vector3f playspaceOrigin = Eigen::Vector3f(0, 0, 0);
	
	/* Rotation and Translation matrices used in calibration */
	Eigen::Matrix<float, 3, 3> rotationMatrix = Eigen::Matrix3f::Zero();
	Eigen::Matrix<float, 3, 1> translationVector = Eigen::Vector3f::Zero();

	/* All trackers which should be added: W,L,R */
	std::vector<K2STracker> trackerVector{
		K2STracker(), K2STracker(), K2STracker()
	}; // This way we're gonna auto-init filters

	double radPlayspaceOffset = 0.f; // LookAt Kinect in radians
	bool flipSkeleton = true; // If skelton should be flipped
};

extern Settings kinectSettings;

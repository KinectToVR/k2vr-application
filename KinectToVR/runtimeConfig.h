#pragma once
#include <openvr.h>
#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <KinectHandlerBase.h>
#include <any>
#include <array>

class runtimeConfig
{
public:
	/* Variables that will be used at runtime to save current state of things */
	/* controller 0 is right and 1 is left, general indexing: R->L */
	Eigen::Quaternionf headsetOrientation = Eigen::Quaternionf(1, 0, 0, 0);
	Eigen::Vector3f headsetPosition = Eigen::Vector3f(0, 0, 0);
	vr::TrackedDevicePose_t controllerPose[2] = {vr::TrackedDevicePose_t(), vr::TrackedDevicePose_t()};

	/* We store data in arrays of kinectOne size, we're getting only 20 if we are using x360
		and use full array if xOne. Not so memory-saving but rally simplifies works */
	glm::vec4 jointPositions[25];
	glm::quat boneOrientations[25];
	JointTrackingState trackingStates[25];

	std::array<Eigen::Vector3f, 3> positionalOffsetsBackup{{Eigen::Vector3f(), Eigen::Vector3f(), Eigen::Vector3f()}};
	std::array<Eigen::Quaternionf, 3> orientationOffsetsBackup{
		{Eigen::Quaternionf(), Eigen::Quaternionf(), Eigen::Quaternionf()}
	};

	/* Other variables that are necessary to run program successfully */
	int kinectVersion, controllerID[2], vrFrameRate;
	std::string kinectState;
	bool initialised = false, started = false,
	     controllerTriggerPressed[2] = {false, false},
	     controllerGripPressed[2] = {false, false},
	     isSkeletonTracked = false,
	     isOverlayVisible = false, settingOffsets = false;
	float controllerTrackpadPose[2][2] = {{0.f, 0.f}, {0.f, 0.f}};
};

/* Create instance in header, to get it shared */
extern runtimeConfig process;

extern glm::vec3 pExchangeG(vr::HmdVector3d_t vrVector);
extern glm::vec3 pExchangeG(Eigen::Vector3f eigenVector);
extern glm::vec3 pExchangeG(vr::HmdMatrix34_t mat);

extern glm::quat pExchangeQG(Eigen::Quaternionf eigenQuat);
extern glm::quat pExchangeQG(vr::HmdQuaternion_t vrQuat);
extern glm::quat pExchangeQG(vr::HmdMatrix34_t matrix);

extern Eigen::Quaternionf pExchangeQE(vr::HmdQuaternion_t vrQuat);
extern Eigen::Quaternionf pExchangeQE(glm::quat glmQuat);
extern Eigen::Quaternionf pExchangeQE(vr::HmdMatrix34_t matrix);

extern vr::HmdVector3d_t pExchangeV(Eigen::Vector3f eigenVector);
extern vr::HmdVector3d_t pExchangeV(glm::vec3 glmVector);
extern vr::HmdVector3d_t pExchangeV(vr::HmdMatrix34_t mat);

extern Eigen::Vector3f pExchangeE(vr::HmdVector3d_t vrVector);
extern Eigen::Vector3f pExchangeE(glm::vec3 glmVector);
extern Eigen::Vector3f pExchangeE(vr::HmdMatrix34_t mat);

extern vr::HmdQuaternion_t pExchangeQV(glm::quat glmQuat);
extern vr::HmdQuaternion_t pExchangeQV(Eigen::Quaternionf eigenQuat);
extern vr::HmdQuaternion_t pExchangeQV(vr::HmdMatrix34_t matrix);

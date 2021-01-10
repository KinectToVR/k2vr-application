#pragma once
#include "ITrackingDevice.h"
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

enum ITrackingDeviceType
{
	K2_KinectV1,
	K2_KinectV2,
	K2_PSMoveService,
	K2_Other,
	K2_Unknown
};

typedef int JointTrackingState, TrackingDeviceType;
class TrackingDeviceBase : public ITrackingDevice {
public:
	TrackingDeviceBase() {

	}
	~TrackingDeviceBase() {

	}

	virtual void initialize() {};
	virtual void shutdown() {};
	virtual void update() {};

	virtual HRESULT getStatusResult() { return E_NOTIMPL; }
	virtual std::string statusResultString(HRESULT stat) { return "statusResultString behaviour not defined"; };
	
	/* Handler for Tracking Joints, num 25 */
	glm::vec4 jointPositions[25] = { glm::vec4() };
	glm::quat boneOrientations[25] = { glm::quat() };
	
	JointTrackingState trackingStates[25] = { 0 };
	TrackingDeviceType deviceType = K2_Unknown;
	
	bool isSkeletonTracked = false;
};

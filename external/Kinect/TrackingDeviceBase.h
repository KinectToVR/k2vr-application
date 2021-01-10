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
	glm::quat jointOrientations[25] = { glm::quat() };
	
	JointTrackingState trackingStates[25] = { 0 };
	TrackingDeviceType deviceType = K2_Unknown;
	
	bool isSkeletonTracked = false;
};

#pragma once
#include "ITrackingDevice.h"
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <KinectToVR_API.h>

class TrackingDeviceBase : public ITrackingDevice {
public:
	TrackingDeviceBase() {

	}
	~TrackingDeviceBase() {

	}

	// These 3 functions are critical
	// One should be called before launching k2vr
	// Update and shutdown is called by the k2vr itself
	virtual void initialize() {};
	virtual void shutdown() {};
	virtual void update() {};

	virtual HRESULT getStatusResult() { return E_NOTIMPL; }
	virtual std::string statusResultString(HRESULT stat) { return "statusResultString behaviour not defined"; };
	
	/*
	 * Handler for Tracking Joints, num 25
	 * You must update these during device's update function call
	 */
	glm::vec4 jointPositions[25] = { glm::vec4() };
	glm::quat jointOrientations[25] = { glm::quat() };
	JointTrackingState trackingStates[25] = { 0 };

	// Device type, specified in K2API
	TrackingDeviceType deviceType = K2_Unknown;

	// Helper bool for knowing if skeleton't tracked
	bool isSkeletonTracked = false;
};

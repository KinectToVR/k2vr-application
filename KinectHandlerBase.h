#pragma once
#include "IKinectHandler.h"
#include <glm/vec4.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

typedef int JointTrackingState;
class KinectHandlerBase : public IKinectHandler {
public:
	KinectHandlerBase() {

	}
	~KinectHandlerBase() {

	}

	virtual void initialise() {};
	virtual void shutdown() {};
	virtual void initOpenGL() {};

	virtual void initialiseSkeleton() {};
	virtual void initialiseColor() {};
	virtual void initialiseDepth() {};

	virtual void terminateSkeleton() {};
	virtual void terminateColor() {};
	virtual void terminateDepth() {};

	virtual HRESULT getStatusResult() { return E_NOTIMPL; }
	virtual std::string statusResultString(HRESULT stat) { return "statusResultString behaviour not defined"; };

        virtual void update() {};

	int kinectVersion = 0;
	glm::vec4 jointPositions[25] = { glm::vec4() }; //Create quick handler for kinect joints, with xOne number
													//If we will be using x360 we'll just count to <20 instead of <25
	glm::quat boneOrientations[25] = { glm::quat() };
	JointTrackingState trackingStates[25] = { 0 }; //All not tracked
        bool isSkeletonTracked = false;

};

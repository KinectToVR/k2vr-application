#pragma once
#include <Windows.h>
#include <ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <NuiSkeleton.h>

#include "TrackingDeviceBase.h"

class KinectV1Handler : public TrackingDeviceBase
{
	// A representation of the Kinect elements for the v1 api
public:
	KinectV1Handler()
	{
		KinectV1Handler::initialize();
		TrackingDeviceBase::deviceType = K2_KinectV1;
	}

	HANDLE kinectRGBStream = nullptr;
	HANDLE kinectDepthStream = nullptr;
	INuiSensor* kinectSensor = nullptr;
	NUI_SKELETON_FRAME skeletonFrame = {0};

	Vector4 jointPositions[NUI_SKELETON_POSITION_COUNT];
	NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
	NUI_SKELETON_POSITION_TRACKING_STATE jointStates[NUI_SKELETON_POSITION_COUNT];

	void initialize() override;
	void update() override;
	void shutdown() override;

	virtual ~KinectV1Handler()
	{
	}

	HRESULT getStatusResult() override;
	std::string statusResultString(HRESULT stat) override;

private:
	bool initKinect();
	bool acquireKinectFrame(NUI_IMAGE_FRAME& imageFrame, HANDLE& rgbStream, INuiSensor*& sensor);
	void releaseKinectFrame(NUI_IMAGE_FRAME& imageFrame, HANDLE& rgbStream, INuiSensor*& sensor);

	void updateSkeletalData();
	Vector4 zeroKinectPosition(int trackedSkeletonIndex);
};

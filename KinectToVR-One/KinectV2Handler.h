#pragma once
#include "ITrackingDevice.h"
#include "TrackingDeviceBase.h"
#include <Kinect.h>
#include <Windows.h>

class KinectV2Handler : public TrackingDeviceBase
{
public:
	KinectV2Handler()
	{
		KinectV2Handler::initialize();
		TrackingDeviceBase::deviceType = ktvr::K2_KinectV2;
	}

	virtual ~KinectV2Handler()
	{
	}

	IKinectSensor* kinectSensor = nullptr;
	IBodyFrameReader* bodyFrameReader = nullptr;
	IColorFrameReader* colorFrameReader = nullptr;
	IDepthFrameReader* depthFrameReader = nullptr;
	IMultiSourceFrame* multiFrame = nullptr;
	ICoordinateMapper* coordMapper = nullptr;
	BOOLEAN isTracking = false;

	Joint joints[JointType_Count];
	JointOrientation jointOrientations[JointType_Count];
	IBody* kinectBodies[BODY_COUNT];

	HRESULT getStatusResult() override;
	std::string statusResultString(HRESULT stat) override;

	void initialize() override;
	void update() override;
	void shutdown() override;
	
	bool convertColorToDepthResolution = false;
	void onBodyFrameArrived(IBodyFrameReader& sender, IBodyFrameArrivedEventArgs& eventArgs);
	virtual void updateSkeletalData();

private:
	bool initKinect();
	void updateParseFrame();
	void initializeSkeleton();
	void terminateSkeleton();

	WAITABLE_HANDLE h_bodyFrameEvent;
	bool newBodyFrameArrived = false;

	/* For translating Kinect joint enumeration to K2 space */
	int globalIndex[25] = { 3,2,20,4,5,6,7,21,22,8,9,10,11,23,24,1,0,12,13,14,15,16,17,18,19 };
};

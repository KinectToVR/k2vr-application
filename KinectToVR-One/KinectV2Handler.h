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
		TrackingDeviceBase::deviceType = K2_KinectV2;
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
	void updateSkeletalFilters();
	void initializeSkeleton();
	void terminateSkeleton();

	WAITABLE_HANDLE h_bodyFrameEvent;
	bool newBodyFrameArrived = false;
};

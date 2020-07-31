#pragma once
#include "IKinectHandler.h"
#include "KinectHandlerBase.h"
#include <Kinect.h>
#include <Windows.h>

class KinectV2Handler : public KinectHandlerBase {
public:
    KinectV2Handler() {
        KinectV2Handler::initialise();
        KinectV2Handler::initOpenGL();
        KinectHandlerBase::kinectVersion = 2;
    }
    virtual ~KinectV2Handler() {}

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

    virtual HRESULT getStatusResult() override;
    virtual std::string statusResultString(HRESULT stat) override;

    virtual void initialise() override;

    virtual void initialiseSkeleton() override;
    virtual void terminateSkeleton() override;
    virtual void initOpenGL() override;
    virtual void update() override;


    bool convertColorToDepthResolution = false;
    void onBodyFrameArrived(IBodyFrameReader& sender, IBodyFrameArrivedEventArgs& eventArgs);
    virtual void updateSkeletalData();

private:
    bool initKinect();
    void updateSkeletalFilters();

    WAITABLE_HANDLE h_bodyFrameEvent;
    bool newBodyFrameArrived = false;

};

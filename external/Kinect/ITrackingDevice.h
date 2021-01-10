#pragma once
#include <string>
#include <vector>
#include <windows.h>

class ITrackingDevice {
    // Interface base for K2 Tracking Device
public:
    virtual ~ITrackingDevice() {}
    
    virtual void initialize() = 0;
    virtual HRESULT getStatusResult() = 0;
    virtual std::string statusResultString(HRESULT stat) = 0;

    virtual void update() = 0;
    bool isInitialized() { return initialized; }
    bool isZeroed() { return zeroed; }
    bool zeroed = false;

protected:
    bool initialized= false;

    class FailedKinectInitialisation : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Failure to initialize the Kinect sensor. Is it plugged in and supplied with power?";
        }
    } FailedKinectInitialisation;
private:
};

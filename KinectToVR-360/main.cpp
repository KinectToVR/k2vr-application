#include <QApplication>
#include <KinectV1Handler.h>
#include <KinectToVR.h>

int main(int argc, char* argv[])
{
	// Open and init the Kinect sensor
	KinectV1Handler Kinect;
	Kinect.initialize();

	// Return main loop, passing the Kinect object
	return run(argc, argv, Kinect);
}

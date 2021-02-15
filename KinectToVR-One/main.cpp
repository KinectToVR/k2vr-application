#include <QApplication>
#include <KinectV2Handler.h>
#include <KinectToVR.h>

int main(int argc, char* argv[])
{
	// Open and init the Kinect sensor
	KinectV2Handler Kinect;
	Kinect.initialize();

	// Return main loop, passing the Kinect object
	return run(argc, argv, Kinect);
}

#include <QApplication>
#include <KinectV2Handler.h>
#include <KinectToVR.h>

int main(int argc, char* argv[])
{
	KinectV2Handler Kinect;
	Kinect.initialize();


	return run(argc, argv, Kinect);
}

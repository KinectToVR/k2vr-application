#include <QApplication>
#include <KinectV1Handler.h>
#include <KinectToVR.h>

int main(int argc, char *argv[])
{
    initLogging();

    KinectV1Handler Kinect;
    Kinect.initialiseSkeleton();


    return run(argc, argv, Kinect);
}

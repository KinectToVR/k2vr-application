#include <QApplication>
#include <KinectV2Handler.h>
#include <KinectToVR.h>

int main(int argc, char *argv[])
{

    KinectV2Handler Kinect;
    Kinect.initialiseSkeleton();


    return run(argc, argv, Kinect);
}

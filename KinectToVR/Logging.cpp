#include <Logging.h>

/* Configure logging and print first message */
KINECTTOVR_LIB void initLogging()
{
	/* Initialize logging */
	google::InitGoogleLogging("KinectToVR");
	/* Log everything >=INFO to same file */
	google::SetLogDestination(google::GLOG_INFO, "KinectToVR");
	
	FLAGS_logbufsecs = 1; //Set max timeout
	FLAGS_minloglevel = google::GLOG_INFO;
	
	LOG(INFO) << "~~~KinectToVR new logging session begins here!~~~";
}

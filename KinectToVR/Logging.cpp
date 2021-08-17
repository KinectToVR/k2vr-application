#include <Logging.h>
#include "getVariable.h"

/* Configure logging and print first message */
KINECTTOVR_LIB void initLogging()
{
	// ktvr::GetK2AppDataFileDir will create all directories by itself

	/* Initialize logging */
	google::InitGoogleLogging(ktvr::GetK2AppDataLogFileDir("KinectToVR_APP").c_str());
	/* Log everything >=INFO to same file */
	google::SetLogDestination(google::GLOG_INFO, ktvr::GetK2AppDataLogFileDir("KinectToVR_APP").c_str());
	google::SetLogFilenameExtension(".log");

	FLAGS_logbufsecs = 0; //Set max timeout
	FLAGS_minloglevel = google::GLOG_INFO;
	
	LOG(INFO) << "~~~KinectToVR new logging session begins here!~~~";
}

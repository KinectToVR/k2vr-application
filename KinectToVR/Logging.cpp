#include <Logging.h>

/* Configure logging and print first message */
KINECTTOVR_LIB void initLogging()
{
	char path[MAX_PATH];
	HMODULE hm = NULL;

	/* Get dll's path (not exe which loaded it nor execution location) */
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&initLogging, &hm);
	GetModuleFileNameA(hm, path, sizeof path);

	/* Log to logs/ directory */
	std::string _path(path);
	_path.insert(_path.rfind("KinectToVR"), "logs\\");

	/* Create logs/ directory */
	CreateDirectoryA(
		_path.substr(0, _path.rfind("\\KinectToVR")).c_str(), NULL);

	/* Remove .dll substring from log name */
	_path.erase(_path.rfind(".dll"), std::string(".dll").length());

	/* Initialize logging */
	google::InitGoogleLogging(_path.c_str());
	/* Log everything >=INFO to same file */
	google::SetLogDestination(google::GLOG_INFO, _path.c_str());
	
	FLAGS_logbufsecs = 0; //Set max timeout
	FLAGS_minloglevel = google::GLOG_INFO; // Log everything
	
	LOG(INFO) << "~~~KinectToVR new logging session begins here!~~~";
}

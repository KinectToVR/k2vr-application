#include <iostream>
#include <Windows.h>
#include <thread>
#include <openvr_driver.h>
#include <K2Tracker.h>
#include <K2ServerDriver.h>

namespace k2_driver
{
	class K2ServerProvider : public vr::IServerTrackedDeviceProvider
	{
		K2ServerDriver m_ServerDriver;

	public:
		int port = 7135;
		
		K2ServerProvider()
		{
			LOG(INFO) << "Provider component creation has started";
		}

		vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override
		{
			// NOTE 1: use the driver context. Sets up a big set of globals
			VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
			LOG(INFO) << "Driver context init success";

			// Initialize communication with K2API
			const int initCode = m_ServerDriver.init_ServerDriver("tcp://127.0.0.1:" + std::to_string(port));
			LOG(INFO) << "Driver's networking server init code: " + 
				std::to_string(initCode);

			if (initCode == 0) {
				LOG(INFO) << "OpenVR ServerDriver init success";
				return vr::VRInitError_None;
			}
			
			LOG(ERROR) << "OpenVR ServerDriver init failure! Aborting...";
			return vr::VRInitError_Driver_Failed;
		}

		void Cleanup() override
		{
			LOG(INFO) << "Cleaning up...";
			m_ServerDriver.setActive(false);
		}

		const char* const* GetInterfaceVersions() override
		{
			return vr::k_InterfaceVersions;
		}

		// It's running every frame
		void RunFrame() override
		{

		}

		bool ShouldBlockStandbyMode() override
		{
			return false;
		}

		void EnterStandby() override
		{
		}

		void LeaveStandby() override
		{
		}
	};
}

bool g_bExiting = false;

class K2WatchdogDriver : public vr::IVRWatchdogProvider
{
public:
	K2WatchdogDriver()
	{
		m_pWatchdogThread = nullptr;
	}

	vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override;
	void Cleanup() override;

private:
	std::thread* m_pWatchdogThread;
};

void WatchdogThreadFunction()
{
	while (!g_bExiting)
	{
		vr::VRWatchdogHost()->WatchdogWakeUp(vr::TrackedDeviceClass_HMD);
		std::this_thread::sleep_for(std::chrono::microseconds(500));
	}
}

vr::EVRInitError K2WatchdogDriver::Init(vr::IVRDriverContext* pDriverContext)
{
	VR_INIT_WATCHDOG_DRIVER_CONTEXT(pDriverContext);
	LOG(INFO) << "Watchdog init has started...";

	g_bExiting = false;
	m_pWatchdogThread = new std::thread(WatchdogThreadFunction);
	if (!m_pWatchdogThread)
	{
		//Log watchdog error and set init=false
		return vr::VRInitError_Driver_Failed;
	}

	return vr::VRInitError_None;
}

void K2WatchdogDriver::Cleanup()
{
	g_bExiting = true;
	if (m_pWatchdogThread)
	{
		m_pWatchdogThread->join();
		delete m_pWatchdogThread;
		m_pWatchdogThread = nullptr;
	}
}

extern "C" __declspec(dllexport) void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode)
{
	char path[MAX_PATH];
	HMODULE hm = NULL;

	/* Get dll's path (not exe which loaded it nor execution location) */
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&HmdDriverFactory, &hm);
	GetModuleFileNameA(hm, path, sizeof path);
	
	/* Log to logs/ directory */
	std::string _path(path);
	_path.insert(_path.find("driver_KinectToVR.dll"), "logs\\");

	/* Create logs/ directory */
	CreateDirectoryA(
		_path.substr(0, _path.find("\\driver_KinectToVR.dll")).c_str(), NULL);
	
	/* Initialize logging */
	google::InitGoogleLogging(_path.c_str());
	/* Log everything >=INFO to same file */
	google::SetLogDestination(google::GLOG_INFO, _path.c_str());
	google::SetLogFilenameExtension(".log");
	
	FLAGS_logbufsecs = 0; //Set max timeout
	FLAGS_minloglevel = google::GLOG_INFO;

	LOG(INFO) << "~~~KinectToVR OpenVR Driver new logging session begins here!~~~";
	LOG(INFO) << "Interface name: " << pInterfaceName;

	static k2_driver::K2ServerProvider k2_server_provider;
	static K2WatchdogDriver k2_watchdog_driver;

	LOG(INFO) << "KinectToVR OpenVR Driver will try to run on port " + std::to_string(k2_server_provider.port);

	if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName))
	{
		return &k2_server_provider;
	}
	if (0 == strcmp(vr::IVRWatchdogProvider_Version, pInterfaceName))
	{
		return &k2_watchdog_driver;
	}

	return nullptr;
}

#include "K2Tracker.h"
#include <openvr_driver.h>
#include <string>
#include <thread>
#include <KinectToVR_API.h>

K2Tracker::K2Tracker(K2Objects::K2TrackerBase const& tracker_base)
{
	_serial = tracker_base.data.serial;
	_role = tracker_base.data.role;
	_active = tracker_base.data.isActive;

	_pose = { 0 };
	_pose.poseIsValid = true;
	_pose.result = vr::TrackingResult_Running_OK;
	_pose.deviceIsConnected = tracker_base.data.isActive;

	_pose.qWorldFromDriverRotation.w = 1;
	_pose.qWorldFromDriverRotation.x = 0;
	_pose.qWorldFromDriverRotation.y = 0;
	_pose.qWorldFromDriverRotation.z = 0;
	_pose.qDriverFromHeadRotation.w = 1;
	_pose.qDriverFromHeadRotation.x = 0;
	_pose.qDriverFromHeadRotation.y = 0;
	_pose.qDriverFromHeadRotation.z = 0;

	_pose.vecPosition[0] = 0;
	_pose.vecPosition[1] = 0;
	_pose.vecPosition[2] = 0;
}

std::string K2Tracker::get_serial() const
{
	return _serial;
}


void K2Tracker::update()
{
	_pose.poseIsValid = _active;
	_pose.deviceIsConnected = _active;
	vr::VRServerDriverHost()->TrackedDevicePoseUpdated(_index, _pose, sizeof _pose);
}

void K2Tracker::set_pose(K2Objects::K2TrackerPose const& pose)
{
	_pose.vecPosition[0] = pose.position.x;
	_pose.vecPosition[1] = pose.position.y;
	_pose.vecPosition[2] = pose.position.z;

	_pose.qRotation.w = pose.orientation.w;
	_pose.qRotation.x = pose.orientation.x;
	_pose.qRotation.y = pose.orientation.y;
	_pose.qRotation.z = pose.orientation.z;
}

void K2Tracker::set_pose(K2Objects::K2TrackerPose const& pose, const double _millisFromNow)
{
	std::thread([&]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(_millisFromNow)));
			set_pose(pose);
		}).detach();
}

void K2Tracker::set_data(K2Objects::K2TrackerData const& data)
{
	if (!_added)
	{
		_serial = data.serial;
		_role = data.role;

		// Not spawning, just pose validity
		_active = data.isActive;
	}
}

void K2Tracker::set_data(K2Objects::K2TrackerData const& data, const double _millisFromNow)
{
	std::thread([&]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(_millisFromNow)));
			set_data(data);
		}).detach();
}

void K2Tracker::set_state(bool state)
{
	_active = state;
}

bool K2Tracker::spawn()
{
	try {
		if (!_added && !_serial.empty())
		{
			// Add device to OpenVR devices list
			vr::VRServerDriverHost()->TrackedDeviceAdded(_serial.c_str(), vr::TrackedDeviceClass_GenericTracker, this);
			_added = true;

			// Force tracker to update at 120fps until exit
			std::thread([&]()
				{
					// For limiting loop's iterations/s
					using clock = std::chrono::steady_clock;
					auto next_frame = clock::now();

					while (true)
					{
						// Add time to wait
						next_frame += std::chrono::milliseconds(1000 / 120);
						// Update tracker's pose
						update();
						//Sleep until next frame
						std::this_thread::sleep_until(next_frame);
					}
				}).detach();
				return true;
		}
	}
	catch (...) {}
	return false;
}

vr::TrackedDeviceIndex_t K2Tracker::get_index() const
{
	return _index;
}

void K2Tracker::process_event(const vr::VREvent_t& event)
{
}

vr::EVRInitError K2Tracker::Activate(vr::TrackedDeviceIndex_t index)
{
	// Save the device index
	_index = index;

	// Get the properties handle for our controller
	_props = vr::VRProperties()->TrackedDeviceToPropertyContainer(_index);

	// Set our universe ID
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_CurrentUniverseId_Uint64, 2);

	// Create components
	vr::VRDriverInput()->CreateBooleanComponent(_props, "/input/system/click", &_components._system_click);
	vr::VRDriverInput()->CreateHapticComponent(_props, "/output/haptic", &_components._haptic);

	// Register all properties
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_TrackingSystemName_String, "lighthouse");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_ModelNumber_String, "Vive Tracker Pro MV");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_SerialNumber_String, _serial.c_str());
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_RenderModelName_String, "{htc}vr_tracker_vive_1_0");

	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_WillDriftInYaw_Bool, false);
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_ManufacturerName_String, "HTC");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_TrackingFirmwareVersion_String,
		"1541800000 RUNNER-WATCHMAN$runner-watchman@runner-watchman 2018-01-01 FPGA 512(2.56/0/0) BL 0 VRC 1541800000 Radio 1518800000");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_HardwareRevision_String,
		"product 128 rev 2.5.6 lot 2000/0/0 0");

	vr::VRProperties()->SetStringProperty(_props, vr::Prop_ConnectedWirelessDongle_String, "D0000BE000");
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_DeviceIsWireless_Bool, true);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_DeviceIsCharging_Bool, false);
	vr::VRProperties()->SetFloatProperty(_props, vr::Prop_DeviceBatteryPercentage_Float, 1.f);

	vr::HmdMatrix34_t l_transform = { -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f };
	vr::VRProperties()->SetProperty(_props, vr::Prop_StatusDisplayTransform_Matrix34, &l_transform,
		sizeof(vr::HmdMatrix34_t),
		vr::k_unHmdMatrix34PropertyTag);

	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_Firmware_UpdateAvailable_Bool, false);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_Firmware_ManualUpdate_Bool, false);
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_Firmware_ManualUpdateURL_String,
		"https://developer.valvesoftware.com/wiki/SteamVR/HowTo_Update_Firmware");
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_HardwareRevision_Uint64, 2214720000);
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_FirmwareVersion_Uint64, 1541800000);
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_FPGAVersion_Uint64, 512);
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_VRCVersion_Uint64, 1514800000);
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_RadioVersion_Uint64, 1518800000);
	vr::VRProperties()->SetUint64Property(_props, vr::Prop_DongleVersion_Uint64, 8933539758);

	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_DeviceProvidesBatteryStatus_Bool, true);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_DeviceCanPowerOff_Bool, true);
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_Firmware_ProgrammingTarget_String, _serial.c_str());
	vr::VRProperties()->SetInt32Property(_props, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_GenericTracker);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_Firmware_ForceUpdateRequired_Bool, false);
	//vr::VRProperties()->SetUint64Property(_props, vr::Prop_ParentDriver_Uint64, 8589934597);
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_ResourceRoot_String, "htc");

	std::string l_registeredType("htc/vive_tracker");
	l_registeredType.append(_serial);
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_RegisteredDeviceType_String, l_registeredType.c_str());
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_Identifiable_Bool, false);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_Firmware_RemindUpdate_Bool, false);
	vr::VRProperties()->SetInt32Property(_props, vr::Prop_ControllerRoleHint_Int32, vr::TrackedControllerRole_Invalid);
	vr::VRProperties()->SetInt32Property(_props, vr::Prop_ControllerHandSelectionPriority_Int32, -1);

	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceOff_String,
		"{htc}/icons/tracker_status_off.png");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceSearching_String,
		"{htc}/icons/tracker_status_searching.gif");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceSearchingAlert_String,
		"{htc}/icons/tracker_status_searching_alert.gif");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceReady_String,
		"{htc}/icons/tracker_status_ready.png");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceReadyAlert_String,
		"{htc}/icons/tracker_status_ready_alert.png");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceNotReady_String,
		"{htc}/icons/tracker_status_error.png");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceStandby_String,
		"{htc}/icons/tracker_status_standby.png");
	vr::VRProperties()->SetStringProperty(_props, vr::Prop_NamedIconPathDeviceAlertLow_String,
		"{htc}/icons/tracker_status_ready_low.png");

	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_HasDisplayComponent_Bool, false);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_HasCameraComponent_Bool, false);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_HasDriverDirectModeComponent_Bool, false);
	vr::VRProperties()->SetBoolProperty(_props, vr::Prop_HasVirtualDisplayComponent_Bool, false);

	/*Get tracker role*/
	const std::string role_enum_string = ITrackerType_String.at(static_cast<ITrackerType>(_role));

	/*Update controller type and input path*/
	const std::string input_path =
		"{htc}/input/tracker/" + role_enum_string + "_profile.json";

	vr::VRProperties()->SetStringProperty(_props,
		vr::Prop_InputProfilePath_String, input_path.c_str());
	vr::VRProperties()->SetStringProperty(_props,
		vr::Prop_ControllerType_String, role_enum_string.c_str());

	/*Update tracker's role in menu*/
	vr::VRSettings()->SetString(vr::k_pch_Trackers_Section, _serial.c_str(), 
		ITrackerType_Role_String.at(static_cast<ITrackerType>(_role)));

	return vr::VRInitError_None;
}

void K2Tracker::Deactivate()
{
	// Clear device id
	_index = vr::k_unTrackedDeviceIndexInvalid;
}

void* K2Tracker::GetComponent(const char* component)
{
	// No extra components on this device so always return nullptr
	return nullptr;
}

void K2Tracker::DebugRequest(const char* request, char* response_buffer, uint32_t response_buffer_size)
{
	// No custom debug requests defined
	if (response_buffer_size >= 1)
		response_buffer[0] = 0;
}

void K2Tracker::EnterStandby()
{
	// Enter standby mode
}

vr::DriverPose_t K2Tracker::GetPose()
{
	return _pose;
}

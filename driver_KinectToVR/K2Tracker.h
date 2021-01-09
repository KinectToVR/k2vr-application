#pragma once
#include <string>
#include <openvr_driver.h>
#include <KinectToVR_API.h>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

class K2Tracker : public vr::ITrackedDeviceServerDriver
{
public:

	//K2Tracker() = default;
	virtual ~K2Tracker() = default;

	/*K2Tracker(K2Tracker&&) = delete;
	K2Tracker& operator=(K2Tracker&&) = delete;
	K2Tracker(const K2Tracker&) = delete;
	K2Tracker& operator=(const K2Tracker&) = delete;*/

	/**
	 * \brief Get tracker serial number
	 * \return Returns tracker's serial in std::string
	 */
	[[nodiscard]] std::string get_serial() const;
	
	/**
	 * \brief Get device index in OpenVR
	 * \return OpenVR device index in uint32_t
	 */
	[[nodiscard]] vr::TrackedDeviceIndex_t get_index() const;

	/**
	 * \brief Virtual update void for server driver
	 */
	virtual void update();

	/**
	 * \brief Function processing OpenVR events
	 */
	void process_event(const vr::VREvent_t& event);

	/**
	 * \brief Activate device (called from OpenVR)
	 * \return InitError for OpenVR if we're set up correctly
	 */
	vr::EVRInitError Activate(vr::TrackedDeviceIndex_t index) override;

	/**
	 * \brief Deactivate tracker (remove)
	 */
	void Deactivate() override;

	/**
	 * \brief Get component handle (for OpenVR)
	 */
	void* GetComponent(const char* component) override;

	/**
	 * \brief Handle debug request (not needed/implemented)
	 */
	void DebugRequest(const char* request, char* response_buffer, uint32_t response_buffer_size) override;

	/**
	 * \brief Enter Standby mode
	 */
	void EnterStandby() override;

	/**
	 * \brief Return device's actual pose
	 */
	vr::DriverPose_t GetPose() override;

	/**
	 * \brief Construct new tracker from given tracker base
	 */
	K2Tracker(K2Objects::K2TrackerBase const& tracker_base);

	// Update pose
	void set_pose(K2Objects::K2TrackerPose const& pose);
	void set_pose(K2Objects::K2TrackerPose const& pose, double _millisFromNow);

	// Update data (only if uninitialized)
	void set_data(K2Objects::K2TrackerData const& data);
	void set_data(K2Objects::K2TrackerData const& data, double _millisFromNow);

	void set_state(bool state);
	bool spawn(); // TrackedDeviceAdded

	// Get to know if tracker is activated (added)
	[[nodiscard]] bool is_added() const { return _added; }
	// Get to know if tracker is active (connected)
	[[nodiscard]] bool is_active() const { return _active; }

private:

	// Is tracker added/active
	bool _added = false, _active = false;

	// Stores the openvr supplied device index.
	vr::TrackedDeviceIndex_t _index;

	// Stores the devices current pose.
	vr::DriverPose_t _pose;

	// An identifier for OpenVR for when we want to make property changes to this device.
	vr::PropertyContainerHandle_t _props;

	// A struct for concise storage of all of the component handles for this device.
	struct TrackerComponents
	{
		vr::VRInputComponentHandle_t
			_system_click,
			_haptic;
	};

	TrackerComponents _components;
	std::string _serial, _role;
};

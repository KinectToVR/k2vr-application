#pragma once
#include <string>
#include <chrono>

#include <openvr_driver.h>
#include <KinectToVR_API.h>

class K2Tracker : public vr::ITrackedDeviceServerDriver
{
public:

	virtual ~K2Tracker() = default;

	K2Tracker(K2Tracker&&) = delete;
	K2Tracker& operator=(K2Tracker&&) = delete;
	K2Tracker(const K2Tracker&) = delete;
	K2Tracker& operator=(const K2Tracker&) = delete;

	/**
	 * \brief Get tracker serial number
	 * \return Returns tracker's serial in std::string
	 */
	std::string get_serial() const;

	/**
	 * \brief Virtual update void for server driver
	 */
	virtual void update();

	/**
	 * \brief Get device index in OpenVR
	 * \return OpenVR device index in uint32_t
	 */
	vr::TrackedDeviceIndex_t get_index() const;

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
	 * \brief Construct new tracker from given tracker base
	 */
	K2Tracker(K2Objects::K2TrackerBase const& tracker_base);

	void set_pose(K2Objects::K2TrackerPose pose);
	void set_state(bool state);

private:

	// Stores the openvr supplied device index.
	vr::TrackedDeviceIndex_t _index;

	static vr::DriverPose_t dlpose;
	static std::chrono::milliseconds dlpose_timestamp;

	// Stores the devices current pose.
	vr::DriverPose_t _pose;

	// Stores the timestamp of the pose.
	std::chrono::milliseconds _pose_timestamp;

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

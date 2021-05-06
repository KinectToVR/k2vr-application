#pragma once
#include <openvr.h>
#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <TrackingDeviceBase.h>
#include <array>

class runtimeConfig
{
public:
	/* Variables that will be used at runtime to save current state of things */
	/* controller 0 is right and 1 is left, general indexing: R->L */
	Eigen::Quaternionf headsetOrientation = Eigen::Quaternionf(1, 0, 0, 0);
	Eigen::Vector3f headsetPosition = Eigen::Vector3f(0, 0, 0);
	vr::TrackedDevicePose_t controllerPose[2] = {vr::TrackedDevicePose_t(), vr::TrackedDevicePose_t()};

	/* We store data in arrays of KV2 size, although K2 numering (see enumeration) */
	glm::vec4 jointPositions[25];
	glm::quat boneOrientations[25];
	ktvr::JointTrackingState trackingStates[25];

	std::array<Eigen::Vector3f, 3> positionOffsetsBackup{
		{Eigen::Vector3f(), Eigen::Vector3f(), Eigen::Vector3f()}};
	std::array<Eigen::Quaternionf, 3> orientationOffsetsBackup{
		{Eigen::Quaternionf(), Eigen::Quaternionf(), Eigen::Quaternionf()}
	};

	/* Other variables that are necessary to run program successfully */
	ktvr::TrackingDeviceType k_deviceType;
	std::string sensorState = "E_UNKNOWN";

	int controllerID[2], vrFrameRate; // R, L, IndexInvalid if not connected
	bool initialized = false, started = false,
	     controllerTriggerPressed[2] = {false, false},
	     controllerGripPressed[2] = {false, false},
	     isSkeletonTracked = false,
	     isOverlayVisible = false, settingOffsets = false;
	float controllerTrackpadPose[2][2] = {{0.f, 0.f}, {0.f, 0.f}};

	/* Interfacing with the k2api */
	long long pingTime = 0, parsingTime = 0,
		lastLoopTime = 0;
	int pingCheckingThreadsNumber = 0;
	const int maxPingCheckingThreads = 3;
};

/* Create instance in header, to get it shared */
extern runtimeConfig process;

/**
 * \brief This template will let us convert between different types
 * \tparam ret What type should be returned from function
 * \tparam T Class of the parameter 'in'
 * \param in Parameter, object which will be 'converted'
 * \return Returns 'in' converted to 'ret' return type
 */
template <typename ret, class T> auto p_cast_type(const T& in)
{
	/* If somehow same */
	if constexpr (std::is_same<ret, T>::value) return in;

	/* To glm Quaternion */
	else if constexpr (std::is_same<ret, glm::quat>::value && std::is_same<T, Eigen::Quaternionf>::value)
		return glm::quat(in.w(), in.x(), in.y(), in.z());
	
	else if constexpr (std::is_same<ret, glm::quat>::value && std::is_same<T, vr::HmdQuaternion_t>::value)
		return glm::quat(in.w, in.x, in.y, in.z);

	/* To Eigen Quaternion */
	else if constexpr (std::is_same<ret, Eigen::Quaternionf>::value && std::is_same<T, glm::quat>::value ||
		std::is_same<ret, Eigen::Quaternionf>::value && std::is_same<T, vr::HmdQuaternion_t>::value)
		return Eigen::Quaternionf(in.w, in.x, in.y, in.z);

	/* To OpenVR Quaternion */
	else if constexpr (std::is_same<ret, vr::HmdQuaternion_t>::value && std::is_same<T, glm::quat>::value)
		return vr::HmdQuaternion_t{ in.w, in.x, in.y, in.z };
	
	else if constexpr (std::is_same<ret, vr::HmdQuaternion_t>::value && std::is_same<T, Eigen::Quaternionf>::value)
		return vr::HmdQuaternion_t{ in.w(), in.x(), in.y(), in.z() };

	/* To glm vec3 */
	else if constexpr (std::is_same<ret, glm::vec3>::value && std::is_same<T, Eigen::Vector3f>::value)
		return glm::vec3(in.x(), in.y(), in.z());
	
	else if constexpr (std::is_same<ret, glm::vec3>::value && std::is_same<T, vr::HmdVector3d_t>::value)
		return glm::vec3(in.x, in.y, in.z);
	
	else if constexpr (std::is_same<ret, glm::vec3>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
		return glm::vec3(in.m[0][3], in.m[1][3], in.m[2][3]);

	/* To Eigen Vector3f */
	else if constexpr (std::is_same<ret, Eigen::Vector3f>::value && std::is_same<T, glm::vec3>::value ||
		std::is_same<ret, Eigen::Vector3f>::value && std::is_same<T, vr::HmdVector3d_t>::value)
		return Eigen::Vector3f(in.x, in.y, in.z);
	
	else if constexpr (std::is_same<ret, Eigen::Vector3f>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
		return Eigen::Vector3f(in.m[0][3], in.m[1][3], in.m[2][3]);

	/* To OpenVR HmdVector3d_t */
	else if constexpr (std::is_same<ret, vr::HmdVector3d_t>::value && std::is_same<T, glm::vec3>::value)
		return vr::HmdVector3d_t{ in.x, in.y, in.z };
	
	else if constexpr (std::is_same<ret, vr::HmdVector3d_t>::value && std::is_same<T, Eigen::Vector3f>::value)
		return vr::HmdVector3d_t{ in.x(), in.y(), in.z() };
	
	else if constexpr (std::is_same<ret, vr::HmdVector3d_t>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
		return vr::HmdVector3d_t{ in.m[0][3], in.m[1][3], in.m[2][3] };

	/* From OpenVR Matrix to OpenVR Quaternion */
	else if constexpr (std::is_same<ret, vr::HmdQuaternion_t>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
	{
		auto q = vr::HmdQuaternion_t{ 1.,0.,0.,0. };
		q.w = sqrt(fmax(0, 1 + in.m[0][0] + in.m[1][1] + in.m[2][2])) / 2;
		q.x = sqrt(fmax(0, 1 + in.m[0][0] - in.m[1][1] - in.m[2][2])) / 2;
		q.y = sqrt(fmax(0, 1 - in.m[0][0] + in.m[1][1] - in.m[2][2])) / 2;
		q.z = sqrt(fmax(0, 1 - in.m[0][0] - in.m[1][1] + in.m[2][2])) / 2;
		q.x = copysign(q.x, in.m[2][1] - in.m[1][2]);
		q.y = copysign(q.y, in.m[0][2] - in.m[2][0]);
		q.z = copysign(q.z, in.m[1][0] - in.m[0][1]);
		return q;
	}

	/* From OpenVR Matrix to glm Quaternion */
	else if constexpr (std::is_same<ret, glm::quat>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
		return p_cast_type<glm::quat>(p_cast_type<vr::HmdQuaternion_t>(in));

	/* From OpenVR Matrix to Eigen Quaternion */
	else if constexpr (std::is_same<ret, Eigen::Quaternionf>::value && std::is_same<T, vr::HmdMatrix34_t>::value)
		return p_cast_type<Eigen::Quaternionf>(p_cast_type<vr::HmdQuaternion_t>(in));
}

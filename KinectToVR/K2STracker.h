#pragma once
#include <KinectToVR_API.h>
#include "LowPassFilter.h"
#include "KalmanFilter.h"

/* tracking filter option enumeration for getting it easier */
enum positionTrackingFilterOptions
{
	t_PositionTrackingFilter_LERP, // Interpolation
	t_PositionTrackingFilter_LowPass, // Low pass filter
	t_PositionTrackingFilter_Kalman, // Extended Kalman
	t_NoPositionTrackingFilter // Filter Off
};

/* tracking filter option enumeration for getting it easier */
enum orientationTrackingFilterOptions
{
	t_OrientationTrackingFilter_SLERP, // Spherical interpolation
	t_OrientationTrackingFilter_SLERP_Slow, // Spherical interpolation, but slower
	t_NoOrientationTrackingFilter // Filter Off
};

class K2STracker : public ktvr::K2TrackerBase
{
public:

	// Position filter update option
	int positionTrackingFilterOption = t_NoPositionTrackingFilter,
		orientationTrackingFilterOption = t_NoOrientationTrackingFilter;

	// Should values be overwritten (for default ones)
	bool overwriteDefaultSerial = false;
	
	// Internal data offset
	glm::vec3 positionOffset = glm::vec3(0,0,0);
	glm::quat orientationOffset = glm::quat(1,0,0,0);

	// For internal filters
	void updatePositionFilters(),
		updateOrientationFilters();

	// Get filtered data
	// By default, the saved filter is selected,
	// and to select it, the filter number must be < 0
	template<int filter = -1>
	[[nodiscard]] glm::vec3 getFilteredPosition() const
	{
		int m_filter = filter;
		if (filter < 0)
			m_filter = positionTrackingFilterOption;

		switch (m_filter)
		{
		default:
			return pose.position;
		case t_PositionTrackingFilter_LERP:
			return LERPPosition;
		case t_PositionTrackingFilter_LowPass:
			return lowPassPosition;
		case t_PositionTrackingFilter_Kalman:
			return kalmanPosition;
		case t_NoPositionTrackingFilter:
			return pose.position;
		}
	}

	// Get filtered data
	// By default, the saved filter is selected,
	// and to select it, the filter number must be < 0
	template<int filter = -1>
	[[nodiscard]] glm::quat getFilteredOrientation() const
	{
		int m_filter = filter;
		if (filter < 0)
			m_filter = orientationTrackingFilterOption;

		switch (m_filter)
		{
		default:
			return pose.orientation;
		case t_OrientationTrackingFilter_SLERP:
			return SLERPOrientation;
		case t_OrientationTrackingFilter_SLERP_Slow:
			return SLERPSlowOrientation;
		case t_NoOrientationTrackingFilter:
			return pose.orientation;
		}
	}

	// Get filtered data
	// By default, the saved filter is selected,
	// and to select it, the filter number must be < 0
	// Additionally, this adds the offsets
	template<int filter = -1>
	[[nodiscard]] glm::vec3 getFullPosition() const
	{
		return getFilteredPosition<filter>() + positionOffset;
	}

	// Get filtered data
	// By default, the saved filter is selected,
	// and to select it, the filter number must be < 0
	// Additionally, this adds the offsets
	template<int filter = -1>
	[[nodiscard]] glm::quat getFullOrientation() const
	{
		return getFilteredOrientation<filter>() * orientationOffset;
	}

	// Get filtered data
	// By default, the saved filter is selected,
	// and to select it, the filter number must be < 0
	// Additionally, this adds the offsets
	// Offset will be added after translation
	template<int filter = -1>
	[[nodiscard]] glm::vec3 getFullCalibratedPosition
	(
		Eigen::Matrix<float, 3, 3> rotationMatrix,
		Eigen::Matrix<float, 3, 1> translationVector,
		Eigen::Vector3f calibration_origin = Eigen::Vector3d::Zero()
	) const
	{
		// Construct the current pose
		Eigen::Vector3f m_pose(
			getFilteredPosition<filter>().x,
			getFilteredPosition<filter>().y,
			getFilteredPosition<filter>().z
		);

		// Construct the calibrated pose
		Eigen::Matrix<float, 3, Eigen::Dynamic> m_pose_calibrated = 
			(rotationMatrix * (m_pose - calibration_origin)).
			colwise() + translationVector + calibration_origin;

		// Construct the calibrated pose in glm
		glm::vec3 calibrated_pose_gl(
			m_pose_calibrated(0),
			m_pose_calibrated(1),
			m_pose_calibrated(2)
		);

		// Return the calibrated pose with offset
		return calibrated_pose_gl + positionOffset;
	}

	// Get tracker base
	// This is for updating the server with
	// exclusive filtered data from K2STracker
	// By default, the saved filter is selected
	// Offsets are added inside called methods
	template<int pos_filter = -1, int ori_filter = -1>
	[[nodiscard]] ktvr::K2TrackerBase getTrackerBase
	(
		Eigen::Matrix<float, 3, 3> rotationMatrix,
		Eigen::Matrix<float, 3, 1> translationVector,
		Eigen::Vector3f calibration_origin
	) const
	{
		// Check if matrices are empty
		bool uncalibrated =
			rotationMatrix.isZero() &&
			translationVector.isZero() &&
			calibration_origin.isZero();

		// Construct the return type
		K2TrackerBase tracker_base(
			ktvr::K2TrackerPose(
				getFullOrientation<ori_filter>(),
				uncalibrated ? getFullPosition<pos_filter>() :
				getFullCalibratedPosition<pos_filter>
				(rotationMatrix, translationVector, calibration_origin)
			),
			ktvr::K2TrackerData(
				data.serial, static_cast<ktvr::ITrackerType>(data.role), data.isActive
			)
		);

		// Add id and return
		tracker_base.id = id;
		return tracker_base;
	}

	// Get tracker base
	// This is for updating the server with
	// exclusive filtered data from K2STracker
	// By default, the saved filter is selected
	// Offsets are added inside called methods
	template<int pos_filter = -1, int ori_filter = -1>
	[[nodiscard]] ktvr::K2TrackerBase getTrackerBase() const
	{
		// Construct the return type
		K2TrackerBase tracker_base(
			ktvr::K2TrackerPose(
				getFullOrientation<ori_filter>(),
				getFullPosition<pos_filter>()
			),
			ktvr::K2TrackerData(
				data.serial, static_cast<ktvr::ITrackerType>(data.role), data.isActive
			)
		);

		// Add id and return
		tracker_base.id = id;
		return tracker_base;
	}

	// Initialize all internal filters
	void initAllFilters();

	// Internal filters' datas
	glm::vec3 kalmanPosition = glm::vec3(),
		lowPassPosition = glm::vec3(), LERPPosition = glm::vec3();

	glm::quat SLERPOrientation = glm::quat(),
		SLERPSlowOrientation = glm::quat();

	// LERP datas backup
	glm::vec3 lastLERPPosition;
	glm::quat lastSLERPOrientation,
		lastSLERPSlowOrientation;

	// Default constructors
	K2STracker()
	{
		initAllFilters();
		lastLERPPosition = pose.position;
		lastSLERPOrientation = pose.orientation;
		lastSLERPSlowOrientation = pose.orientation;
	};
	~K2STracker() = default;
	
	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar& boost::serialization::make_nvp("tracker_base",
			boost::serialization::base_object<K2TrackerBase>(*this))
			& BOOST_SERIALIZATION_NVP(positionTrackingFilterOption)
			& BOOST_SERIALIZATION_NVP(orientationTrackingFilterOption)
			& BOOST_SERIALIZATION_NVP(overwriteDefaultSerial)
			& BOOST_SERIALIZATION_NVP(positionOffset)
			& BOOST_SERIALIZATION_NVP(orientationOffset);
	}

private:
	/* Position filters */

	// Internal position filters
	LowPassFilter lowPassFilter[3] = {
				LowPassFilter(7.1, 0.005),
				LowPassFilter(7.1, 0.005),
				LowPassFilter(7.1, 0.005)
	};

	// Internal Kalman filter, must be initialized
	KalmanFilter kalmanFilter[3] = {
			KalmanFilter(), KalmanFilter(), KalmanFilter()
	};
};


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
	t_OrientationTrackingFilter_LowPass, // Low pass filter
	t_OrientationTrackingFilter_Kalman, // Extended Kalman
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
	glm::vec3 positionOffset = glm::vec3();
	glm::quat orientationOffset = glm::quat();

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
		case t_OrientationTrackingFilter_LowPass:
			return lowPassOrientation;
		case t_OrientationTrackingFilter_Kalman:
			return kalmanOrientation;
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

	// Initialize all internal filters
	void initAllFilters();

	// Internal filters' datas
	glm::vec3 kalmanPosition = glm::vec3(),
		lowPassPosition = glm::vec3(), LERPPosition = glm::vec3();

	glm::quat kalmanOrientation = glm::quat(),
		lowPassOrientation = glm::quat(), SLERPOrientation = glm::quat();

	// LERP datas backup
	glm::vec3 lastLERPPosition;
	glm::quat lastSLERPOrientation;

	// Default constructors
	K2STracker()
	{
		initAllFilters();
		lastLERPPosition = pose.position;
		lastSLERPOrientation = pose.orientation;
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

	/* Orientation filters */

	// Internal position filters
	LowPassFilter rot_lowPassFilter[4] = {
				LowPassFilter(7.1, 0.005),
				LowPassFilter(7.1, 0.005),
				LowPassFilter(7.1, 0.005),
				LowPassFilter(7.1, 0.005)
	};

	// Internal Kalman filter, must be initialized
	KalmanFilter rot_kalmanFilter[4] = {
			KalmanFilter(), KalmanFilter(), KalmanFilter(), KalmanFilter()
	};
};

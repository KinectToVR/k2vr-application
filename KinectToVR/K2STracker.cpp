#include "K2STracker.h"

template <typename _Scalar>
Eigen::Vector3<_Scalar> lerp(const Eigen::Vector3<_Scalar>& x, const Eigen::Vector3<_Scalar>& y, float a) {
	return x * (1.0 - a) + y * a; // Same as glm::mix - https://glm.g-truc.net/0.9.4/api/a00129.html#ga3f64b3986efe205cf30300700667e761
}

void K2STracker::updatePositionFilters()
{
	/* Update the Kalman filter */
	Eigen::VectorXd y[3] = {
			Eigen::VectorXd(1), Eigen::VectorXd(1), Eigen::VectorXd(1)
	}, u(1); // c == 1

	y[0] << pose.position.x();
	y[1] << pose.position.y();
	y[2] << pose.position.z();
	u << 0; // zero control input

	for (int i = 0; i < 3; i++) {
		kalmanFilter[i].predict(u);
		kalmanFilter[i].update(y[i]);
	}

	kalmanPosition = Eigen::Vector3f(
		kalmanFilter[0].state().x(),
		kalmanFilter[1].state().x(),
		kalmanFilter[2].state().x());

	/* Update the LowPass filter */
	lowPassPosition = Eigen::Vector3f(
		lowPassFilter[0].update(pose.position.x()),
		lowPassFilter[1].update(pose.position.y()),
		lowPassFilter[2].update(pose.position.z()));

	/* Update the LERP (mix) filter */
	LERPPosition = lerp(lastLERPPosition, pose.position, .5f);
	lastLERPPosition = pose.position; // Backup the position
}

void K2STracker::updateOrientationFilters()
{
	/* Update the SLERP filter */
	SLERPOrientation = lastSLERPOrientation.slerp(0.6, pose.orientation);
	lastSLERPOrientation = pose.orientation; // Backup the orientation

	/* Update the Slower SLERP filter */
	SLERPSlowOrientation = lastSLERPSlowOrientation.slerp(0.3, pose.orientation);
	lastSLERPSlowOrientation = pose.orientation; // Backup the orientation
}

void K2STracker::initAllFilters()
{
	/* Position filters */
	for (auto& filter : kalmanFilter)
		filter.init();
}

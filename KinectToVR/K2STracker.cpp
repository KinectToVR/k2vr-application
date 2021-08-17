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
	};

	y[0] << pose.position.x();
	y[1] << pose.position.y();
	y[2] << pose.position.z();

	for (int i = 0; i < 3; i++)
		kalmanFilter[i].update(y[i]);

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
	LERPPosition = lerp(lastLERPPosition, pose.position, .9f);
	lastLERPPosition = pose.position; // Backup the position
}

void K2STracker::updateOrientationFilters()
{
	/* Update the SLERP filter */
	SLERPOrientation = lastSLERPOrientation.slerp(0.8, pose.orientation);
	lastSLERPOrientation = pose.orientation; // Backup the position

	/* Update the Slower SLERP filter */
	SLERPSlowOrientation = lastSLERPSlowOrientation.slerp(0.5, pose.orientation);
	lastSLERPSlowOrientation = pose.orientation; // Backup the position
}

void K2STracker::initAllFilters()
{
	int n = 3, m = 1; // Number of measurements & statements
	double dt = 1.0 / 20.0, t[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	Eigen::MatrixXd A(n, n), C(m, n), Q(n, n), R(m, m), P(n, n);

	A << 1, dt, 0, 0, 1, dt, 0, 0, 1;
	C << 1, 0, 0;

	Q << .05, .05, .0, .05, .05, .0, .0, .0, .0;
	R << 5;
	P << .1, .1, .1, .1, 10000, 10, .1, 10, 100;

	Eigen::VectorXd x0(n);
	x0 << .0, .0, .0;

	/* Position filters */
	for (int i = 0; i < 3; i++)
	{
		kalmanFilter[i] = KalmanFilter(dt, A, C, Q, R, P);
		kalmanFilter[i].init(.0, x0);
	}
}

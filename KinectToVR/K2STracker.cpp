#include "K2STracker.h"

void K2STracker::updatePositionFilters()
{
	/* Update the Kalman filter */
	Eigen::VectorXd y[3] = {
			Eigen::VectorXd(1), Eigen::VectorXd(1), Eigen::VectorXd(1)
	};

	y[0] << pose.position.x;
	y[1] << pose.position.y;
	y[2] << pose.position.z;

	for (int i = 0; i < 3; i++)
		kalmanFilter[i].update(y[i]);

	kalmanPosition = glm::vec3(
		kalmanFilter[0].state().x(),
		kalmanFilter[1].state().x(),
		kalmanFilter[2].state().x());

	/* Update the LowPass filter */
	lowPassPosition = glm::vec3(
		lowPassFilter[0].update(pose.position.x),
		lowPassFilter[1].update(pose.position.y),
		lowPassFilter[2].update(pose.position.z));

	/* Update the LERP (mix) filter */
	LERPPosition = glm::mix(lastLERPPosition, pose.position, .5f);
	lastLERPPosition = pose.position; // Backup the position
}

void K2STracker::updateOrientationFilters()
{
	/* Update the Kalman filter */
	Eigen::VectorXd y[4] = {
			Eigen::VectorXd(1), Eigen::VectorXd(1),
			Eigen::VectorXd(1), Eigen::VectorXd(1)
	};

	// We're multiplying per 100 to make it reliable
	// - just the thing to make numbers bigger
	y[0] << 100.0 * pose.orientation.w;
	y[1] << 100.0 * pose.orientation.x;
	y[2] << 100.0 * pose.orientation.y;
	y[3] << 100.0 * pose.orientation.z;

	for (int i = 0; i < 4; i++)
		rot_kalmanFilter[i].update(y[i]);

	kalmanOrientation = glm::quat(
		rot_kalmanFilter[0].state().x() / 100.0,
		rot_kalmanFilter[1].state().x() / 100.0,
		rot_kalmanFilter[2].state().x() / 100.0,
		rot_kalmanFilter[3].state().x() / 100.0);

	/* Update the LowPass filter */
	lowPassOrientation = glm::quat(
		rot_lowPassFilter[0].update(
			100.0 * pose.orientation.w) / 100.0,
		rot_lowPassFilter[1].update(
			100.0 * pose.orientation.x) / 100.0,
		rot_lowPassFilter[2].update(
			100.0 * pose.orientation.y) / 100.0,
		rot_lowPassFilter[3].update(
			100.0 * pose.orientation.z) / 100.0);

	/* Update the SLERP filter */
	SLERPOrientation = glm::slerp(lastSLERPOrientation, pose.orientation, .5f);
	lastSLERPOrientation = pose.orientation; // Backup the position
}

void K2STracker::initAllFilters()
{
	int n = 3, m = 1; // Number of measurements & statements
	double dt = 1.0 / 30, t[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
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

	/* Orientation filters */
	for (int i = 0; i < 4; i++)
	{
		rot_kalmanFilter[i] = KalmanFilter(dt, A, C, Q, R, P);
		rot_kalmanFilter[i].init(.0, x0);
	}
}

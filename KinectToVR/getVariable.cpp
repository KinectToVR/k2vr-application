#include "getVariable.h"
#include <runtimeConfig.h>

getVariable::getVariable(QObject* parent) : QObject(parent)
{
}

Q_INVOKABLE

int getVariable::get(const QString& msg, const int& arg, const int& arg1) const
{
	/* If 'arg' if less than 25, we can assume that QML
	 * is going to ask us for joints' positions.
	 * Then check if message says the same
	 */
	
	if (arg < 25)
	{
		if (msg == "JOINT_POSE")
		{
			switch (arg1)
			{
			case 0:
				return process.jointPositions[arg].x * 1000;
			case 1:
				return process.jointPositions[arg].y * 1000;
			case 2:
				return process.jointPositions[arg].z * 1000;
			default:
				return 0;
			}
		}
		if (msg == "JOINT_ROT")
		{
			switch (arg1)
			{
			case 0:
				return process.boneOrientations[arg].w * 1000;
			case 1:
				return process.boneOrientations[arg].x * 1000;
			case 2:
				return process.boneOrientations[arg].y * 1000;
			case 3:
				return process.boneOrientations[arg].z * 100;
			default:
				return 0;
			}
		}
		if (msg == "JOINT_STATE")
			return process.trackingStates[arg];
	}
	
	if (msg == "SENSOR_STATE")
		return process.sensorState == "S_OK";

	if (msg == "SKELETON_STATE")
		return process.isSkeletonTracked;

	if (msg == "VISIBLE")
		return process.isOverlayVisible;

	if (msg == "FLIP_SKELETON")
		return kinectSettings.flipSkeleton;

	return 0; //If message couldn't be processed, return 0
}

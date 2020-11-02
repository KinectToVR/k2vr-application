#include "runtimeConfig.h"

/* Create instance here, to be visible in every file
	and usable everywhenre, when header is included */
runtimeConfig process = runtimeConfig();

/*
Helper class for keeping process variables and delaying with multiple libraries at once
Provides exchange between glm, eigen and openvr basic types
*/

Eigen::Vector3f pExchangeE(vr::HmdVector3d_t vrVector)
{
	return Eigen::Vector3f(vrVector.v[0],
	                       vrVector.v[1], vrVector.v[2]);
}

glm::vec3 pExchangeG(vr::HmdVector3d_t vrVector)
{
	return glm::vec3(vrVector.v[0],
	                 vrVector.v[1], vrVector.v[2]);
}

vr::HmdVector3d_t pExchangeV(Eigen::Vector3f eigenVector)
{
	return vr::HmdVector3d_t{
		eigenVector.x(),
		eigenVector.y(), eigenVector.z()
	};
}

vr::HmdVector3d_t pExchangeV(glm::vec3 glmVector)
{
	return vr::HmdVector3d_t{
		glmVector.x,
		glmVector.y, glmVector.z
	};
}

vr::HmdVector3d_t pExchangeV(vr::HmdMatrix34_t mat)
{
	return vr::HmdVector3d_t{
		mat.m[0][3], mat.m[1][3], mat.m[2][3]
	};
}

glm::quat pExchangeQG(vr::HmdQuaternion_t vrQuat)
{
	return glm::quat(vrQuat.w, vrQuat.x, vrQuat.y, vrQuat.z);
}

Eigen::Quaternionf pExchangeQE(vr::HmdQuaternion_t vrQuat)
{
	return Eigen::Quaternionf(vrQuat.w, vrQuat.x, vrQuat.y, vrQuat.z);
}

vr::HmdQuaternion_t pExchangeQV(glm::quat glmQuat)
{
	return vr::HmdQuaternion_t{
		glmQuat.w, glmQuat.x, glmQuat.y, glmQuat.z
	};
}

vr::HmdQuaternion_t pExchangeQV(Eigen::Quaternionf eigenQuat)
{
	return vr::HmdQuaternion_t{
		eigenQuat.w(), eigenQuat.x(), eigenQuat.y(), eigenQuat.z()
	};
}

glm::quat pExchangeQG(Eigen::Quaternionf eigenQuat)
{
	return glm::quat(eigenQuat.w(), eigenQuat.x(), eigenQuat.y(), eigenQuat.z());
}

Eigen::Quaternionf pExchangeQE(glm::quat glmQuat)
{
	return Eigen::Quaternionf(glmQuat.w, glmQuat.x, glmQuat.y, glmQuat.z);
}

glm::vec3 pExchangeG(Eigen::Vector3f eigenVector)
{
	return glm::vec3(eigenVector.x(), eigenVector.y(), eigenVector.z());
}

Eigen::Vector3f pExchangeE(glm::vec3 glmVector)
{
	return Eigen::Vector3f(glmVector.x, glmVector.y, glmVector.z);
}

glm::vec3 pExchangeG(vr::HmdMatrix34_t mat)
{
	return glm::vec3(
		mat.m[0][3], mat.m[1][3], mat.m[2][3]
	);
}

Eigen::Vector3f pExchangeE(vr::HmdMatrix34_t mat)
{
	return Eigen::Vector3f(
		mat.m[0][3], mat.m[1][3], mat.m[2][3]
	);
}

vr::HmdQuaternion_t pExchangeQV(vr::HmdMatrix34_t matrix)
{
	vr::HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
	return q;
}

Eigen::Quaternionf pExchangeQE(vr::HmdMatrix34_t mat)
{
	return pExchangeQE(pExchangeQV(mat));
}

glm::quat pExchangeQG(vr::HmdMatrix34_t mat)
{
	return pExchangeQG(pExchangeQV(mat));
}

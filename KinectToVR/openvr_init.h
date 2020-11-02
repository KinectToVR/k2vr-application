#pragma once

namespace openvr_init
{
	enum class OpenVrInitializationType
	{
		Overlay,
		Utility,
	};

	void initializeOpenVR(OpenVrInitializationType initType);
} // namespace openvr_init

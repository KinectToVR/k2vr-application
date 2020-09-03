#pragma once
#include <openvr.h>
#include <easylogging++.h>
#include "paths.h"

namespace input
{
	/*!
	Initializes IVRInput to point at the correct manifest. Can't be done in the
	constructor body since objects being initialized require this to be set.
	*/
	class Manifest
	{
	public:
		Manifest()
		{
			const auto actionManifestPath
				= paths::binaryDirectoryFindFile("action_manifest.json");

			if (!actionManifestPath.has_value())
			{
				LOG(ERROR) << u8"アクションマニフェストが見つかりませんでした、イニシャライズをしていません！";
				return;
			}

			const auto validManifestPath = actionManifestPath->c_str();

			auto error = vr::VRInput()->SetActionManifestPath(validManifestPath);
			if (error != vr::EVRInputError::VRInputError_None)
			{
				LOG(ERROR) << u8"アクションマニフェストパスの構成中にエラーが発生した！「"
					<< validManifestPath << "」OpenVRエラー：" << error;
			}
		}

		// Explicitly deleted because this object should do nothing but run the
		// constructor during initialization.
		// Destructor left blank on purpose since there are no terminating calls.
		~Manifest() {}
		// Copy constructor
		Manifest(const Manifest&) = delete;
		// Copy assignment
		Manifest& operator=(const Manifest&) = delete;
		// Move constructor
		Manifest(const Manifest&&) = delete;
		// Move assignment
		Manifest& operator=(const Manifest&&) = delete;
	};

} // namespace input

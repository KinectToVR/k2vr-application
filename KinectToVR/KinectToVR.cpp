#include <KinectToVR.h>
#include <SkeletonImageProvider.h>

/* For interfacing */
QObject* quickObj;

KINECTTOVR_LIB int run(int argc, char* argv[], TrackingDeviceBase& tracking_device)
{
	/* Initialize logging */
	initLogging();

	/* Variables for overlay controller */
	const bool desktopMode = false,
		noSound = true;
	//bool noManifest = true;

	/* set app attributes and create basic instance */
	LOG(INFO) << "Initializing QGuiApplication...";
	QGuiApplication::setAttribute(Qt::AA_Use96Dpi);
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication main(argc, argv);

	main.setOrganizationName(
		application_strings::applicationOrganizationName);
	main.setApplicationName(application_strings::applicationName);
	main.setApplicationDisplayName(
		application_strings::applicationDisplayName);
	main.setApplicationVersion(
		application_strings::applicationVersionString);
	
	/* read saved settings from file,
	 * trycatch already implemented */
	kinectSettings.readSettings();

	/* update 3 default trackers <Settings.h> */
	for (int i = 0; i < 3; i++)
	{
		// Update the serial if we're not overwriting
		if (!kinectSettings.trackerVector.at(i).overwriteDefaultSerial)
			kinectSettings.trackerVector.at(i).data.serial = "/devices/KinectToVR/LHR-CB9AD1T" + std::to_string(i);

		// Switch the rest
		switch (i)
		{
		case 0: // Waist
			kinectSettings.trackerVector.at(i).data.role = ktvr::Tracker_Waist;
			break;
		case 1: // LFoot
			kinectSettings.trackerVector.at(i).data.role = ktvr::Tracker_LeftFoot;
			break;
		case 2: // RFoot
			kinectSettings.trackerVector.at(i).data.role = ktvr::Tracker_RightFoot;
			break;
		}
	}

	/* Initialize OpenVR */
	initializeOpenVR(
		openvr_init::OpenVrInitializationType::Overlay);

	/* register signal handler for qml - for buttons and other stuff */
	signalHandler cppHandler;
	getVariable getData;
	QQmlEngine qmlEngine;
	LOG(INFO) << "Initializing QML Context for OpenVR overlay...";
	qmlEngine.rootContext()->setContextProperty(QStringLiteral("_cppContext"), &cppHandler);

	/* register types for qml - getting variables from program */
	LOG(INFO) << "Registering QML Types for OpenVR overlay...";
	qmlEngine.rootContext()->setContextProperty(QStringLiteral("_get"), &getData);

	/* register types for qml - sending images to qml */
	LOG(INFO) << "Registering QML Types for imaging...";
	qmlEngine.addImageProvider(QLatin1String("SkeletonImage"), new SkeletonImageProvider);

	/* Create and initialize overlay controller */
	OverlayController controller(desktopMode, noSound, qmlEngine, tracking_device);

	/* Create QML component for handling basic gui */
	QQmlComponent component(&qmlEngine, QUrl("qrc:/kMainWindow.qml"));
	auto errors = component.errors();
	for (auto& e : errors)
		LOG(ERROR) << "Encountered QML error: " << e.toString().toStdString();
	quickObj = component.create();

	/* Finally, set overlay widget object */
	LOG(INFO) << "Setting up OpenVR overlay widget...";
	controller.SetWidget(qobject_cast<QQuickItem*>(quickObj),
		application_strings::applicationDisplayName,
		application_strings::applicationKey);

	/* Update device label (Will be replaced with std::format later) */
	quickObj->findChild<QObject*>("deviceName")->setProperty("text",
		tracking_device.deviceType == ktvr::K2_PSMoveService ? "PSMoveService (PSMS)" :
		tracking_device.deviceType == ktvr::K2_KinectV1 ? "Kinect for Xbox 360 (V1)" :
		tracking_device.deviceType == ktvr::K2_KinectV2 ? "Kinect for Xbox One (V2)" :
		"Unknown Device (E_UNKNOWN)");

	/* Setup ui with saved defines */
	LOG(INFO) << "Updating overlay's settings with saved ones...";
	quickObj->findChild<QObject*>("flipCheckBox")->setProperty("checkState",
		kinectSettings.flipSkeleton
		? Qt::Checked
		: Qt::Unchecked);

	// Update spinboxes with trackers
	updateQSpinboxes(
		kinectSettings.trackerVector.at(0),
		kinectSettings.trackerVector.at(1),
		kinectSettings.trackerVector.at(2)
	);

	/* combo boxes */
	LOG(INFO) << "Choosing tracking options...";
	quickObj->findChild<QObject*>("hipsComboBox")->
		setProperty("currentIndex", kinectSettings.waistOrientationTrackingOption);
	quickObj->findChild<QObject*>("feetComboBox")->
		setProperty("currentIndex", kinectSettings.feetOrientationTrackingOption);
	quickObj->findChild<QObject*>("filterComboBox")->
		setProperty("currentIndex", kinectSettings.globalPositionTrackingFilterOption);

	/* Set information about what device are we using */
	process.k_deviceType = tracking_device.deviceType;

	/* Create error handler and connect with openvr system */
	vr::EVRInitError vrError = vr::VRInitError_None;
	vr::IVRSystem* p_VRSystem = VR_Init(&vrError, vr::VRApplication_Background);

	/* Exit if initerror is not NONE, we don't want to get a crash */
	if (vrError != vr::VRInitError_None)
	{
		LOG(ERROR) << "Critical error! Couldn't connect to VRSystem, error code: " + boost::lexical_cast<std::string>(vrError);
		return -1;
	}

	/* Scan for playspace origin that is not 0,0,0,0RAD for more see openvr docs */
	LOG(INFO) << "Scanning for non-default space origin...";
	kinectSettings.playspaceOrigin = p_cast_type<glm::vec3>(p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose());
	double yaw = std::atan2(p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose().m[0][2],
		p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose().m[2][2]);
	if (yaw < 0.0) yaw = 2 * M_PI + yaw;
	kinectSettings.radPlayspaceOffset = yaw;

	LOG_IF(INFO, yaw != 0.0) << "Found space yaw offset: " + std::to_string(yaw);

	/* Scan for controllers and get they're ids, get vr framerate for handling main loop */
	process.controllerID[0] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(
		vr::ETrackedControllerRole::TrackedControllerRole_RightHand);
	process.controllerID[1] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(
		vr::ETrackedControllerRole::TrackedControllerRole_LeftHand);
	process.vrFrameRate = p_VRSystem->GetFloatTrackedDeviceProperty(0, vr::Prop_DisplayFrequency_Float);

	/* Update program with potential new version, if detected */
	if (controller.newVersionDetected())
	{
		quickObj->findChild<QObject*>("newVersionPopup")->setProperty("visible", true);
		quickObj->findChild<QObject*>("infoIcon")->setProperty("visible", true);

		quickObj->findChild<QObject*>("updateNumberLabel")->setProperty("text",
			QString("KinectToVR v") +
			QString::number(controller.m_remoteVersionMajor) + "." +
			QString::number(controller.m_remoteVersionMinor) + "." +
			QString::number(controller.m_remoteVersionPatch));

		quickObj->findChild<QObject*>("currentVersionLabel")->setProperty("text",
			QString("KinectToVR v") +
			QString::number(controller.m_localVersionMajor) + "." +
			QString::number(controller.m_localVersionMinor) + "." +
			QString::number(controller.m_localVersionPatch));

		quickObj->findChild<QObject*>("updateInfoLabel")->
			setProperty("text", controller.m_updateMessage);
	}

	/* Main application thread: read kinect and process events */
	std::thread([&]
		{
			/* For limiting loop 'fps' */
			using clock = std::chrono::steady_clock;
			auto next_frame = clock::now();

			/* We have finished setup */
			process.started = true;
			LOG(INFO) << "Configuring done, starting up main thread...";

			while (true)
			{
				/* Check if we have vr framerate, not to divide by 0 and,
					if there is no vr running on hmd, run at 30 fps*/
				next_frame += std::chrono::milliseconds(1000 /
					(process.vrFrameRate >= 30 && process.vrFrameRate <= 140 ? process.vrFrameRate : 30));


				/* If we are currently working on offsets, then update
					process variables with QSpinboxes' values */
				if (process.settingOffsets)
					updateQSpinboxes(
						kinectSettings.trackerVector.at(0),
						kinectSettings.trackerVector.at(1),
						kinectSettings.trackerVector.at(2),
						true);

				/* Grab controller ids in every loop, then we can remove reconnect function,
					check for vr framerate changed every loop*/
				process.controllerID[0] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(
					vr::ETrackedControllerRole::TrackedControllerRole_RightHand);
				process.controllerID[1] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(
					vr::ETrackedControllerRole::TrackedControllerRole_LeftHand);
				process.vrFrameRate = p_VRSystem->GetFloatTrackedDeviceProperty(0,
					vr::Prop_DisplayFrequency_Float);


				/* Get all devices pose in big array, instead of checking for one at time */
				vr::TrackedDevicePose_t vrDevicesPose[vr::k_unMaxTrackedDeviceCount];
				p_VRSystem->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0,
					vrDevicesPose, vr::k_unMaxTrackedDeviceCount);

				/* Push headset position to runtime variables, will be used then */
				process.headsetPosition = p_cast_type<Eigen::Vector3f>(vrDevicesPose[0].mDeviceToAbsoluteTracking);
				process.headsetOrientation = p_cast_type<Eigen::Quaternionf>(vrDevicesPose[0].mDeviceToAbsoluteTracking);

				/* Process controllers input and position (if they are connected) */
				for (int id = 0; id < 2; id++)
				{
					if (static_cast<unsigned>(process.controllerID[id]) != vr::k_unTrackedDeviceIndexInvalid)
					{
						process.controllerPose[id] = vrDevicesPose[process.controllerID[id]];

						/* Get vr controller state and process buttons for each
							for help, and working rules go to openvr docs*/
						vr::VRControllerState_t controllerState;
						p_VRSystem->GetControllerState(process.controllerID[id], &controllerState, sizeof(controllerState));

						process.controllerTriggerPressed[id] = controllerState.rAxis[1].x;
						process.controllerGripPressed[id] =
							controllerState.ulButtonPressed & ButtonMaskFromId(vr::EVRButtonId::k_EButton_Grip);

						process.controllerTrackpadPose[id][0] = controllerState.rAxis[0].x;
						process.controllerTrackpadPose[id][1] = controllerState.rAxis[0].y;
					}
				}


				/* Get tracked joint poses, rots and state and push it to runtime variables */
				tracking_device.update();
				process.isSkeletonTracked = tracking_device.isSkeletonTracked;

				std::copy(std::begin(tracking_device.jointPositions), std::end(tracking_device.jointPositions),
					std::begin(process.jointPositions));
				std::copy(std::begin(tracking_device.jointOrientations), std::end(tracking_device.jointOrientations),
					std::begin(process.boneOrientations));
				std::copy(std::begin(tracking_device.trackingStates), std::end(tracking_device.trackingStates),
					std::begin(process.trackingStates));

				std::this_thread::sleep_until(next_frame); //Sleep until next frame, if time didn't pass yet
			}
		}).detach();


		int app_return = main.exec();
		//Shutdown (total) code goes here
		//Mostly, in case when overlay was 
		//not loaded but app continues to run

		LOG(INFO) << "Shutting down...";
		tracking_device.shutdown(); //turn off kinect
		google::ShutdownGoogleLogging(); //End logging
		return app_return; //Return qt app exectution as result
}

/* Push given value to offsets page */
void updateQSpinboxes(
	K2STracker& tracker_waist,
	K2STracker& tracker_lfoot,
	K2STracker& tracker_rfoot,
	bool set)
{
	if (!set)
	{
		glm::vec3 rot[3] = {
			glm::eulerAngles(tracker_waist.orientationOffset),
			glm::eulerAngles(tracker_lfoot.orientationOffset),
			glm::eulerAngles(tracker_rfoot.orientationOffset)
		};

		/* Shift to degrees */
		for (glm::vec3 &v : rot)
		{
			v.x = glm::degrees(v.x);
			v.y = glm::degrees(v.y);
			v.z = glm::degrees(v.z);
		}

		/* Rotational offsets, XZY in ui, XYZ in code, array order: W->L->R */
		quickObj->findChild<QObject*>("controlW")->setProperty("value", 100 * rot[0].x);
		quickObj->findChild<QObject*>("control1W")->setProperty("value", 100 * rot[0].z);
		quickObj->findChild<QObject*>("control2W")->setProperty("value", 100 * rot[0].y);

		quickObj->findChild<QObject*>("controlL")->setProperty("value", 100 * rot[1].x);
		quickObj->findChild<QObject*>("control1L")->setProperty("value", 100 * rot[1].z);
		quickObj->findChild<QObject*>("control2L")->setProperty("value", 100 * rot[1].y);

		quickObj->findChild<QObject*>("controlR")->setProperty("value", 100 * rot[2].x);
		quickObj->findChild<QObject*>("control1R")->setProperty("value", 100 * rot[2].z);
		quickObj->findChild<QObject*>("control2R")->setProperty("value", 100 * rot[2].y);

		/* Positional offsets, UXYZ in ui, UXYZ in code, array order: W->L->R */
		quickObj->findChild<QObject*>("control3W")->setProperty("value", 100 * tracker_waist.positionOffset.x);
		quickObj->findChild<QObject*>("control4W")->setProperty("value", 100 * tracker_waist.positionOffset.y);
		quickObj->findChild<QObject*>("control5W")->setProperty("value", 100 * tracker_waist.positionOffset.z);

		quickObj->findChild<QObject*>("control3L")->setProperty("value", 100 * tracker_lfoot.positionOffset.x);
		quickObj->findChild<QObject*>("control4L")->setProperty("value", 100 * tracker_lfoot.positionOffset.y);
		quickObj->findChild<QObject*>("control5L")->setProperty("value", 100 * tracker_lfoot.positionOffset.z);

		quickObj->findChild<QObject*>("control3R")->setProperty("value", 100 * tracker_rfoot.positionOffset.x);
		quickObj->findChild<QObject*>("control4R")->setProperty("value", 100 * tracker_rfoot.positionOffset.y);
		quickObj->findChild<QObject*>("control5R")->setProperty("value", 100 * tracker_rfoot.positionOffset.z);
	}
	else
	{
		/* Get variables and compute quaternions from them */
		glm::vec3 rot[3] = {
			glm::vec3(),
			glm::vec3(),
			glm::vec3()
		};

		/* Read offsets from qml, remember that they are XZY, might be as well a for loop */
		rot[0].x = static_cast<float>(quickObj->findChild<QObject*>("controlW")->property("value").toInt()) / 100.f;
		rot[0].z = static_cast<float>(quickObj->findChild<QObject*>("control1W")->property("value").toInt()) / 100.f;
		rot[0].y = static_cast<float>(quickObj->findChild<QObject*>("control2W")->property("value").toInt()) / 100.f;

		rot[1].x = static_cast<float>(quickObj->findChild<QObject*>("controlL")->property("value").toInt()) / 100.f;
		rot[1].z = static_cast<float>(quickObj->findChild<QObject*>("control1L")->property("value").toInt()) / 100.f;
		rot[1].y = static_cast<float>(quickObj->findChild<QObject*>("control2L")->property("value").toInt()) / 100.f;

		rot[2].x = static_cast<float>(quickObj->findChild<QObject*>("controlR")->property("value").toInt()) / 100.f;
		rot[2].z = static_cast<float>(quickObj->findChild<QObject*>("control1R")->property("value").toInt()) / 100.f;
		rot[2].y = static_cast<float>(quickObj->findChild<QObject*>("control2R")->property("value").toInt()) / 100.f;

		/* Shift to radians */
		for(glm::vec3 &v : rot)
		{
			v.x = glm::radians(v.x);
			v.y = glm::radians(v.y);
			v.z = glm::radians(v.z);
		}

		/* Construct quaternions from given euler angles */
		tracker_waist.orientationOffset = glm::quat(rot[0]);
		tracker_lfoot.orientationOffset = glm::quat(rot[1]);
		tracker_rfoot.orientationOffset = glm::quat(rot[2]);

		/* Get positional offsets onto given arrays, might be as well a for loop */
		tracker_waist.positionOffset.x = static_cast<float>(quickObj->findChild<QObject*>("control3W")->property("value").toInt()) / 100.f;
		tracker_waist.positionOffset.y = static_cast<float>(quickObj->findChild<QObject*>("control4W")->property("value").toInt()) / 100.f;
		tracker_waist.positionOffset.z = static_cast<float>(quickObj->findChild<QObject*>("control5W")->property("value").toInt()) / 100.f;

		tracker_lfoot.positionOffset.x = static_cast<float>(quickObj->findChild<QObject*>("control3L")->property("value").toInt()) / 100.f;
		tracker_lfoot.positionOffset.y = static_cast<float>(quickObj->findChild<QObject*>("control4L")->property("value").toInt()) / 100.f;
		tracker_lfoot.positionOffset.z = static_cast<float>(quickObj->findChild<QObject*>("control5L")->property("value").toInt()) / 100.f;

		tracker_rfoot.positionOffset.x = static_cast<float>(quickObj->findChild<QObject*>("control3R")->property("value").toInt()) / 100.f;
		tracker_rfoot.positionOffset.y = static_cast<float>(quickObj->findChild<QObject*>("control4R")->property("value").toInt()) / 100.f;
		tracker_rfoot.positionOffset.z = static_cast<float>(quickObj->findChild<QObject*>("control5R")->property("value").toInt()) / 100.f;
	}
}

void startCalibration(bool automatic)
{
	abortCalibration = false; // Reset

	if (automatic) {
		std::thread([&]
			{
				/* Loop over 3 points and grab positions */
				for (int i = 1; i <= 3; i++) {

					/* Tell user to move somewhere else */
					quickObj->findChild<QObject*>("Autocalib_seconds")->setProperty("text", "~");
					quickObj->findChild<QObject*>("Autocalib_move")->setProperty("text", "Move somewhere else...");
					for (int j = 0; j < 7; j++) { // 3.5 seconds
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
						if (abortCalibration) break; // Check more frequently
					}
					if (abortCalibration) break; // Exit

					/* Capture positions */
					quickObj->findChild<QObject*>("Autocalib_move")->setProperty("text", "Please stand still!");
					for (int j = 3; j >= 0; j--)
					{
						quickObj->findChild<QObject*>("Autocalib_seconds")->setProperty("text", j);
						for (int k = 0; k < 2; k++) {
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
							if (abortCalibration) break; // Check more frequently
						}
						if (abortCalibration) break; // Exit second loop
					}
					// TODO Capture hmd & head positions here to a variable TODO //
					if (abortCalibration) break; // Exit
				}

				/* Look at Kinect part */
				quickObj->findChild<QObject*>("Autocalib_move")->setProperty("text", "Look at the Kinect!");
				for (int j = 3; j >= 0; j--)
				{
					quickObj->findChild<QObject*>("Autocalib_seconds")->setProperty("text", j);
					for (int j = 0; j < 2; j++) {
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
						if (abortCalibration) break; // Check more frequently
					}
					if (abortCalibration) break; // Exit second loop
				}
				// TODO Capture hmd orientation here to a variable TODO //
			
				/* Compose translations */
				// TODO Compose translation and rotation here TODO //
				// TODO Save all gathered data TODO //

				/* Reset labels */
				quickObj->findChild<QObject*>("Autocalib_seconds")->setProperty("text", "~");
				quickObj->findChild<QObject*>("Autocalib_move")->setProperty("text", "");

				/* Notify that we're done */
				if (!abortCalibration) {
					quickObj->findChild<QObject*>("Autocalib_title")->setProperty("text", "Calibration done!");
					std::this_thread::sleep_for(std::chrono::seconds(2));
				}

				/* Finish: close windows */
				QMetaObject::invokeMethod(quickObj->findChild<QObject*>("abortAutoCalibButton"), "closeAutoCalibration");
				quickObj->findChild<QObject*>("Autocalib_title")->setProperty("text", "Follow the directions");

			}).detach();
	}
	else
	{
		std::thread([&]
			{

			}).detach();
	}
}

#include <KinectToVR.h>

INITIALIZE_EASYLOGGINGPP

KINECTTOVR_LIB int run(int argc, char *argv[], KinectHandlerBase& Kinect)
{
        /* Variables for overlay controller */
        bool desktopMode = false;
        bool noSound = true;
        //bool noManifest = true;

        /* set app attributes and create basic instance */
        LOG(INFO) << u8"アプリケーションのイニシャライズ";
        QGuiApplication::setAttribute(Qt::AA_Use96Dpi);
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication main(argc, argv);

        /* register signal handler for qml - for buttons and other stuff */
        signalHandler cppHandler;
        getVariable getData;
        QQmlEngine qmlEngine;
        LOG(INFO) << u8"QMLのコンテキストをOverlayのコントローラーのために登録…";
        qmlEngine.rootContext()->setContextProperty(QStringLiteral("_cppContext"), &cppHandler);

        /* register types for qml - getting variables from program */
        LOG(INFO) << u8"QMLのタイプをOverlayのコントローラーのために登録…";
        qmlEngine.rootContext()->setContextProperty(QStringLiteral("_get"), &getData);

        /* Create and initialise overlay controller */
        OverlayController* controller = OverlayController::createInstance(desktopMode, noSound);
        controller->Init(&qmlEngine);

        /* Create QML component for handling basic gui */
        QQmlComponent component(&qmlEngine, QUrl("qrc:/kMainWindow.qml"));
        auto quickObj = component.create();

        /* Finally, set overlay widget object */
        LOG(INFO) << u8"Overlayのウィジェットのセットアップ…";
        controller->SetWidget(qobject_cast<QQuickItem*>(quickObj),
            OverlayController::applicationName, OverlayController::applicationKey);

        /* Set information about whick kinect version are we using */
        process.kinectVersion = Kinect.kinectVersion;

        /* Create error handler and connect with openvr system */
        vr::EVRInitError vrError = vr::VRInitError_None;
        vr::IVRSystem* p_VRSystem = vr::VR_Init(&vrError, vr::VRApplication_Background);

        /* Exit if initerror is not NONE, we don't want to get a crash */
        if (vrError != vr::VRInitError_None) {
            LOG(ERROR) << u8"クリティカル・エラー！VRSystemにコネクトできませんでした、エラーコード：" + boost::lexical_cast<std::string>(vrError);
            return -1;
        }

        /* Scan for playspace origin that is not 0,0,0,0RAD for more see openvr docs */
        kinectSettings.playspaceOrigin = pExchangeG(p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose());
        double yaw = std::atan2(p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose().m[0][2],
            p_VRSystem->GetRawZeroPoseToStandingAbsoluteTrackingPose().m[2][2]);
        if (yaw < 0.0) yaw = 2 * M_PI + yaw;

        kinectSettings.radPlayspaceOffset = yaw;

        /* Scan for controllers and get they're ids, get vr framerate for handling main loop */
        process.controllerID[0] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole::TrackedControllerRole_RightHand);
        process.controllerID[1] = p_VRSystem->GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole::TrackedControllerRole_LeftHand);
        process.vrFrameRate = p_VRSystem->GetFloatTrackedDeviceProperty(0, vr::Prop_DisplayFrequency_Float);


        /* Main application thread: read kinect and process events */
        std::thread([&] {

            /* For limiting loop 'fps' */
            using clock = std::chrono::steady_clock;
            auto next_frame = clock::now();

            while (true) {

                /* Check if we have vr framerate, not to divide by 0,
                    if there is no vr running on hmd, run at 30 fps*/
                next_frame += std::chrono::milliseconds(1000 /
                    (process.vrFrameRate != 0.f ? process.vrFrameRate : 30));












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
                process.headsetPosition = pExchangeE(vrDevicesPose[0].mDeviceToAbsoluteTracking);
                process.headsetOrientation = pExchangeQE(vrDevicesPose[0].mDeviceToAbsoluteTracking);

                /* Process controllers input and position (if they are connected) */
                for (int id = 0; id < 2; id++) {
                    if ((unsigned int)process.controllerID[id] != vr::k_unTrackedDeviceIndexInvalid) {
                        process.controllerPose[id] = vrDevicesPose[process.controllerID[id]];

                        /* Get vr controller state and process buttons for each
                            for help, and working rules go to openvr docs*/
                        vr::VRControllerState_t controllerState;
                        p_VRSystem->GetControllerState(process.controllerID[id], &controllerState, sizeof(controllerState));

                        process.controllerTriggerPressed[id] = controllerState.rAxis[1].x;
                        process.controllerGripPressed[id] =
                            controllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_Grip);

                        process.controllerTrackpadPose[id][0] = controllerState.rAxis[0].x;
                        process.controllerTrackpadPose[id][1] = controllerState.rAxis[0].y;
                    }
                }




                /* Get kinect joint poses, rots and state and push it to runtime variables */
                Kinect.update();
                process.isSkeletonTracked = Kinect.isSkeletonTracked;

                switch (Kinect.kinectVersion) {
                case 1:
                    std::copy(std::begin(Kinect.jointPositions), std::end(Kinect.jointPositions), std::begin(process.jointPositions));
                    std::copy(std::begin(Kinect.boneOrientations), std::end(Kinect.boneOrientations), std::begin(process.boneOrientations));
                    std::copy(std::begin(Kinect.trackingStates), std::end(Kinect.trackingStates), std::begin(process.trackingStates));
                    break;
                case 2:
                    std::copy(std::begin(Kinect.jointPositions), std::end(Kinect.jointPositions), std::begin(process.jointPositions));
                    std::copy(std::begin(Kinect.boneOrientations), std::end(Kinect.boneOrientations), std::begin(process.boneOrientations));
                    std::copy(std::begin(Kinect.trackingStates), std::end(Kinect.trackingStates), std::begin(process.trackingStates));
                    break;
                }




















































































                std::this_thread::sleep_until(next_frame); //Sleep until next frame, if time didn't pass yet
            }

            }).detach();



















































        return main.exec(); //Return qt app exectution as result
}

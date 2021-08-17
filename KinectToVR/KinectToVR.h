#ifndef KINECTTOVR_H
#define KINECTTOVR_H

#endif // KINECTTOVR_H

#pragma once
#include <QApplication>
#include <overlaycontroller.h>
#include <TrackingDeviceBase.h>
#include <QVBoxLayout>
#include <QQuickView>
#include <QWidget>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QStandardPaths>
#include <buttonHandlers.h>
#include <QQmlProperty>
#include <QQmlContext>
#include <thread>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <getVariable.h>
#include <Logging.h>
#include <Settings.h>
#include <runtimeConfig.h>
#include <boost/lexical_cast.hpp>
#include <array>
#include <KinectToVR_API.h>
#include <K2STracker.h>
#include <EigenUtils.h>

#if defined MAKE_KINECTTOVR_LIB
#define KINECTTOVR_LIB Q_DECL_EXPORT
#else
#define KINECTTOVR_LIB Q_DECL_IMPORT
#endif

// The only exported function, rest should not be called externally
extern "C" KINECTTOVR_LIB int run(int argc, char* argv[], TrackingDeviceBase& tracking_device);

/* Pass quaternion, although spinboxes are using euler angles, should result same */
void updateQSpinboxes(
	K2STracker& tracker_waist,
	K2STracker& tracker_lfoot,
	K2STracker& tracker_rfoot,
	bool set = false);

/* Start the calibration, lower is the bool for aborting */
void startCalibration(bool automatic = true);

/* Test the connection with k2api, blocking */
void testConnection(bool log = false);
inline bool abortCalibration = false;

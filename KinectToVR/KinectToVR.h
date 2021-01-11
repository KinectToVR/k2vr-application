#ifndef KINECTTOVR_H
#define KINECTTOVR_H

#endif // KINECTTOVR_H

#pragma once
#include <QApplication>
#include <overlaycontroller.h>
#include <TrackingDeviceBase.h>
#include <QApplication>
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

#if defined MAKE_KINECTTOVR_LIB
#define KINECTTOVR_LIB Q_DECL_EXPORT
#else
#define KINECTTOVR_LIB Q_DECL_IMPORT
#endif

extern "C" KINECTTOVR_LIB int run(int argc, char* argv[], TrackingDeviceBase& tracking_device);

/* Pass quaternion, although spinboxes are using euler angles, should result same */
void updateQSpinboxes(std::array<Eigen::Vector3f, 3>& pos, std::array<Eigen::Quaternionf, 3>& qrot, bool set = false);
void startCalibration(bool automatic = true);
inline bool abortCalibration = false;
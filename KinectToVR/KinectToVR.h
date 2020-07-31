#ifndef KINECTTOVR_H
#define KINECTTOVR_H

#endif // KINECTTOVR_H

#include <QApplication>
#include <openvroverlaycontroller.h>
#include <KinectHandlerBase.h>
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
#include <openvroverlaycontroller.h>
#include <getVariable.h>
#include <Logging.h>
#include <Settings.h>
#include <runtimeConfig.h>
#include <boost/lexical_cast.hpp>

#if defined MAKE_KINECTTOVR_LIB
#define KINECTTOVR_LIB Q_DECL_EXPORT
#else
 #define KINECTTOVR_LIB Q_DECL_IMPORT
#endif

extern "C" KINECTTOVR_LIB int run(int argc, char *argv[], KinectHandlerBase &Kinect);

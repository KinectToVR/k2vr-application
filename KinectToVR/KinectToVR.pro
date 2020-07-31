QT       += core gui qml quick quickwidgets opengl widgets quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = lib
DEFINES += MAKE_KINECTTOVR_LIB ELPP_NO_DEFAULT_LOG_FILE

SOURCES += \
    KinectToVR.cpp \
    Logging.cpp \
    Settings.cpp \
    buttonHandlers.cpp \
    easylogging++.cc \
    getVariable.cpp \
    openvroverlaycontroller.cpp \
    runtimeConfig.cpp

HEADERS += \
    KinectToVR.h \
    Logging.h \
    Settings.h \
    boost_serialization_eigen.h \
    buttonHandlers.h \
    easylogging++.h \
    getVariable.h \
    openvroverlaycontroller.h \
    runtimeConfig.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L../openvr/lib -lopenvr_api \
    -L../opencv/build/x64/vc15/lib -lopencv_world340 \
    -L../lib64-msvc-14.2

INCLUDEPATH += . ../openvr ../opencv/build/include ..

RESOURCES += \
    resources.qrc

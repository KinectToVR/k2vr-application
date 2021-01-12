QT       += core gui qml quick quickwidgets opengl widgets quickcontrols2 multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS ELPP_NO_DEFAULT_LOG_FILE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = app

SOURCES += \
    KinectV1Handler.cpp \
    main.cpp

DEPENDPATH += . ../KinectToVR $$(KINECTSDK10_DIR)inc
INCLUDEPATH += . .. ../openvr ../opencv/build/include ../KinectToVR $$(KINECTSDK10_DIR)inc

LIBS += -L../build-KinectToVR-Desktop_Qt_5_15_0_MSVC2019_64bit-Release\release -lKinectToVR \
    -L$$(KINECTSDK10_DIR)lib/amd64 -lKinect10 \
    -L../openvr/lib -lopenvr_api \
    -L../opencv/build/x64/vc15/lib -lopencv_world340 \
    -L../lib64-msvc-14.2

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    KinectV1Handler.h

RESOURCES += \
    ../KinectToVR/resources.qrc

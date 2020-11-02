#pragma once

#include <openvr.h>
#include <QtCore/QtCore>
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QObject>
#include <QSettings>
#include <QQmlEngine>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <memory>
#include <atomic>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <KinectHandlerBase.h>
#include <openvr_init.h>
#include <ivrinput.h>
#include <QtMultimedia/QSoundEffect>
#include <Settings.h>
#include <update_rate.h>
#include <runtimeConfig.h>

namespace application_strings
{
	constexpr auto applicationOrganizationName = "KimihikoAkayasaki";
	constexpr auto applicationName = "KinectToVR";
	constexpr const char* applicationKey = "KimihikoAkayasaki.KinectToVR"; //temporary!
	constexpr const char* applicationDisplayName = "KinectToVR";
	constexpr const char* versionCheckUrl
		= "https://raw.githubusercontent.com/OpenVR-Advanced-Settings/" //temporary!
		"OpenVR-AdvancedSettings/master/ver/versioncheck.json";

	constexpr const char* applicationVersionString = "1.0.0";
}

// k_nonVsyncTickRate determines number of ms we wait to force the next event
// loop tick when vsync is too late due to dropped frames.
constexpr int k_nonVsyncTickRate = 20;
constexpr int k_maxCustomTickRate = 999;
constexpr int k_hmdRotationCounterUpdateRate = 7;

class OverlayController : public QObject
{
Q_OBJECT
	Q_PROPERTY(bool m_desktopMode READ isDesktopMode)

private:
	vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle
		= vr::k_ulOverlayHandleInvalid;

	QQuickRenderControl m_renderControl;
	QQuickWindow m_window{&m_renderControl};
	std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
	QOpenGLContext m_openGLContext;
	QOffscreenSurface m_offscreenSurface;

	QTimer m_pumpEventsTimer;
	std::unique_ptr<QTimer> m_pRenderTimer;
	bool m_dashboardVisible = false;

	QPoint m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons = nullptr;

	bool m_desktopMode;
	bool m_noSound;
	bool m_newVersionDetected = false;
	int m_remoteVersionMajor = -1;
	int m_remoteVersionMinor = -1;
	int m_remoteVersionPatch = -1;
	int m_localVersionMajor = -1;
	int m_localVersionMinor = -1;
	int m_localVersionPatch = -1;
	QString m_updateMessage = "";
	QString m_optionalMessage = "";
	QString m_versionCheckText = "";

	QUrl m_runtimePathUrl;
	QSoundEffect m_activationSoundEffect;
	QSoundEffect m_focusChangedSoundEffect;
	QSoundEffect m_alarm01SoundEffect;

	uint64_t m_currentFrame = 0;
	uint64_t m_lastFrame = 0;
	int m_vsyncTooLateCounter = 0;
	int m_customTickRateCounter = 0;
	int m_verifiedCustomTickRateMs = 0;

	input::SteamIVRInput m_actions;
	QJsonDocument m_remoteVersionJsonDocument = QJsonDocument();
	QJsonObject m_remoteVersionJsonObject;

private:
	QPoint getMousePositionForEvent(vr::VREvent_Mouse_t mouse);

	bool m_exclusiveState = false;
	bool m_keyPressOneState = false;
	bool m_keyPressTwoState = false;

public:
	OverlayController(bool desktopMode, bool noSound, QQmlEngine& qmlEngine, KinectHandlerBase& Kinect);
	virtual ~OverlayController();

	void Shutdown();
	Q_INVOKABLE void exitApp();

	bool isDashboardVisible()
	{
		return m_dashboardVisible;
	}

	void SetWidget(QQuickItem* quickItem,
	               const std::string& name,
	               const std::string& key = "");

	bool isDesktopMode()
	{
		return m_desktopMode;
	}

	Q_INVOKABLE QString getVersionString();
	Q_INVOKABLE QUrl getVRRuntimePathUrl();
	Q_INVOKABLE bool soundDisabled();

	const vr::VROverlayHandle_t& overlayHandle();
	const vr::VROverlayHandle_t& overlayThumbnailHandle();
	KinectHandlerBase& Kinect;

	bool pollNextEvent(vr::VROverlayHandle_t ulOverlayHandle,
	                   vr::VREvent_t* pEvent);
	void mainEventLoop();

public slots:
	void renderOverlay();
	void OnRenderRequest();
	void OnTimeoutPumpEvents();

	void showKeyboard(QString existingText, unsigned long userValue = 0);
	void setKeyboardPos();

	void playActivationSound();
	void playFocusChangedSound();
	void playAlarm01Sound(bool loop = false);
	void setAlarm01SoundVolume(float vol);
	void cancelAlarm01Sound();

signals:
	void keyBoardInputSignal(QString input, unsigned long userValue = 0);
};

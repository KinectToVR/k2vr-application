#include "overlaycontroller.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QQuickView>
#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtWidgets/QWidget>
//#include <QMouseEvent>
#include <QSinglePointEvent>
#include <QQuickRenderTarget>
#include <QQuickGraphicsDevice>

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QOpenGLExtraFunctions>
#include <QCursor>
#include <QProcess>
#include <QMessageBox>
#include <iostream>
#include <cmath>
#include <openvr.h>

#include <Logging.h>
#include <paths.h>

namespace utils
{
	constexpr vr::HmdMatrix34_t k_forwardUpMatrix
		= {
			{
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, -1.0f, 0.0f, 0.0f}
			}
	};

	inline vr::HmdMatrix34_t& initRotationMatrix(vr::HmdMatrix34_t& matrix,
		unsigned axisId,
		float angle)
	{
		switch (axisId)
		{
		case 0: // x-axis
			matrix.m[0][0] = 1.0f;
			matrix.m[0][1] = 0.0f;
			matrix.m[0][2] = 0.0f;
			matrix.m[0][3] = 0.0f;
			matrix.m[1][0] = 0.0f;
			matrix.m[1][1] = std::cos(angle);
			matrix.m[1][2] = -std::sin(angle);
			matrix.m[1][3] = 0.0f;
			matrix.m[2][0] = 0.0f;
			matrix.m[2][1] = std::sin(angle);
			matrix.m[2][2] = std::cos(angle);
			matrix.m[2][3] = 0.0f;
			break;
		case 1: // y-axis
			matrix.m[0][0] = std::cos(angle);
			matrix.m[0][1] = 0.0f;
			matrix.m[0][2] = std::sin(angle);
			matrix.m[0][3] = 0.0f;
			matrix.m[1][0] = 0.0f;
			matrix.m[1][1] = 1.0f;
			matrix.m[1][2] = 0.0f;
			matrix.m[1][3] = 0.0f;
			matrix.m[2][0] = -std::sin(angle);
			matrix.m[2][1] = 0.0f;
			matrix.m[2][2] = std::cos(angle);
			matrix.m[2][3] = 0.0f;
			break;
		case 2: // z-axis
			matrix.m[0][0] = std::cos(angle);
			matrix.m[0][1] = -std::sin(angle);
			matrix.m[0][2] = 0.0f;
			matrix.m[0][3] = 0.0f;
			matrix.m[1][0] = std::sin(angle);
			matrix.m[1][1] = std::cos(angle);
			matrix.m[1][2] = 0.0f;
			matrix.m[1][3] = 0.0f;
			matrix.m[2][0] = 0.0f;
			matrix.m[2][1] = 0.0f;
			matrix.m[2][2] = 1.0f;
			matrix.m[2][3] = 0.0f;
			break;
		default:
			break;
		}
		return matrix;
	}

	inline vr::HmdMatrix34_t& matMul33(vr::HmdMatrix34_t& result,
		const vr::HmdMatrix34_t& a,
		const vr::HmdMatrix34_t& b)
	{
		for (unsigned i = 0; i < 3; i++)
		{
			for (unsigned j = 0; j < 3; j++)
			{
				result.m[i][j] = 0.0f;
				for (unsigned k = 0; k < 3; k++)
				{
					result.m[i][j] += a.m[i][k] * b.m[k][j];
				}
			}
		}
		return result;
	}

	inline vr::HmdVector3_t& matMul33(vr::HmdVector3_t& result,
		const vr::HmdMatrix34_t& a,
		const vr::HmdVector3_t& b)
	{
		for (unsigned i = 0; i < 3; i++)
		{
			result.v[i] = 0.0f;
			for (unsigned k = 0; k < 3; k++)
			{
				result.v[i] += a.m[i][k] * b.v[k];
			}
		}
		return result;
	}

	inline vr::HmdVector3_t& matMul33(vr::HmdVector3_t& result,
		const vr::HmdVector3_t& a,
		const vr::HmdMatrix34_t& b)
	{
		for (unsigned i = 0; i < 3; i++)
		{
			result.v[i] = 0.0f;
			for (unsigned k = 0; k < 3; k++)
			{
				result.v[i] += a.v[k] * b.m[k][i];
			}
		}
		return result;
	}
}

int verifyCustomTickRate(const int tickRate)
{
	if (tickRate < 1)
	{
		return 1;
	}
	if (tickRate > k_maxCustomTickRate)
	{
		return k_maxCustomTickRate;
	}

	return tickRate;
}

OverlayController::OverlayController(bool desktopMode,
	bool noSound,
	QQmlEngine& qmlEngine, TrackingDeviceBase& tracking_device)
	: QObject(), m_desktopMode(desktopMode),
	m_noSound(noSound), m_actions(), tracking_device(tracking_device)
{
	// Arbitrarily chosen Max Length of Directory path, should be sufficient for
	// Any set-up
	const uint32_t maxLength = 16192;
	uint32_t requiredLength;

	char tempRuntimePath[maxLength];
	bool pathIsGood
		= vr::VR_GetRuntimePath(tempRuntimePath, maxLength, &requiredLength);

	// Throw Error If over 16k characters in path string
	if (!pathIsGood)
	{
		LOG(ERROR) << "An error occurred while searching for the VR runtime path!";
		uint32_t maxLengthRe = requiredLength;
		LOG(INFO) << "Maximum runtime path length: "
			<< maxLengthRe;
	}

	m_runtimePathUrl = QUrl::fromLocalFile(tempRuntimePath);
	LOG(INFO) << "VR runtime path: " << m_runtimePathUrl.toLocalFile().toStdString();

	/*constexpr auto clickSoundURL = "click.wav";
	const auto activationSoundFile
		= paths::binaryDirectoryFindFile(clickSoundURL);

	if (activationSoundFile.has_value())
	{
		m_activationSoundEffect.setSource(QUrl::fromLocalFile(
			QString::fromStdString((*activationSoundFile))));
		m_activationSoundEffect.setVolume(0.7);
	}
	else
	{
		LOG(ERROR) << "Activation sound file not found, "
			<< clickSoundURL;
	}
	constexpr auto focusChangedSoundURL = "focus.wav";
	const auto focusChangedSoundFile
		= paths::binaryDirectoryFindFile(focusChangedSoundURL);

	if (focusChangedSoundFile.has_value())
	{
		m_focusChangedSoundEffect.setSource(QUrl::fromLocalFile(
			QString::fromStdString((*focusChangedSoundFile))));
		m_focusChangedSoundEffect.setVolume(0.7);
	}
	else
	{
		LOG(ERROR) << "No focus changed sound file found, "
			<< focusChangedSoundURL;
	}

	constexpr auto alarmFileName = "alarm.wav";
	const auto alarm01SoundFile
		= paths::binaryDirectoryFindFile(alarmFileName);

	if (alarm01SoundFile.has_value())
	{
		m_alarm01SoundEffect.setSource(QUrl::fromLocalFile(
			QString::fromStdString((*alarm01SoundFile))));
		m_alarm01SoundEffect.setVolume(1.0);
	}
	else
	{
		LOG(ERROR) << "Alarm sound file not found, " << alarmFileName;
	}*/

	QSurfaceFormat format;
	// Qt's QOpenGLPaintDevice is not compatible with OpenGL versions >= 3.0
	// NVIDIA does not care, but unfortunately AMD does
	// Are subtle changes to the semantics of OpenGL functions actually covered
	// by the compatibility profile, and this is an AMD bug?
	format.setVersion(2, 1);
	// format.setProfile( QSurfaceFormat::CompatibilityProfile );
	format.setDepthBufferSize(16);
	format.setStencilBufferSize(8);
	format.setSamples(16);

	m_openGLContext.setFormat(format);
	if (!m_openGLContext.create())
	{
		throw std::runtime_error("Could not create OpenGL context");
	}

	// create an offscreen surface to attach the context and FBO to
	m_offscreenSurface.setFormat(m_openGLContext.format());
	m_offscreenSurface.create();
	m_openGLContext.makeCurrent(&m_offscreenSurface);

	if (!vr::VROverlay())
	{
		QMessageBox::critical(
			nullptr, "OpenVR Advanced Settings Overlay", "Is OpenVR running?");
		throw std::runtime_error(std::string("No Overlay interface"));
	}

	// Set qml context
	qmlEngine.rootContext()->setContextProperty("applicationVersion",
		getVersionString());
	qmlEngine.rootContext()->setContextProperty("vrRuntimePath",
		getVRRuntimePathUrl());

	// Grab local version number
	QStringList verNumericalString
		= QString(application_strings::applicationVersionString).split("-");
	QStringList verMajorMinorPatchString = verNumericalString[0].split(".");
	m_localVersionMajor = verMajorMinorPatchString[0].toInt();
	m_localVersionMinor = verMajorMinorPatchString[1].toInt();
	m_localVersionPatch = verMajorMinorPatchString[2].toInt();

	// Check for a new version
	checkNewVersion();
}

OverlayController::~OverlayController()
{
	Shutdown();
}

void OverlayController::exitApp()
{
	kinectSettings.saveSettings();
	LOG(INFO) << "Shutting down overlay...";

	tracking_device.shutdown(); //turn off

	Shutdown();
	QApplication::exit();

	LOG(INFO) << "Shut down succeeded!";
	exit(EXIT_SUCCESS);
}

void OverlayController::Shutdown()
{
	disconnect(&m_pumpEventsTimer,
		SIGNAL(timeout()),
		this,
		SLOT(OnTimeoutPumpEvents()));
	m_pumpEventsTimer.stop();

	if (m_pRenderTimer)
	{
		disconnect(&m_renderControl,
			SIGNAL(renderRequested()),
			this,
			SLOT(OnRenderRequest()));
		disconnect(&m_renderControl,
			SIGNAL(sceneChanged()),
			this,
			SLOT(OnRenderRequest()));
		disconnect(m_pRenderTimer.get(),
			SIGNAL(timeout()),
			this,
			SLOT(renderOverlay()));
		m_pRenderTimer->stop();
		m_pRenderTimer.reset();
	}
	m_pFbo.reset();
}

void OverlayController::SetWidget(QQuickItem* quickItem,
	const std::string& name,
	const std::string& key)
{
	if (!m_desktopMode)
	{
		vr::VROverlayError overlayError
			= vr::VROverlay()->CreateDashboardOverlay(
				key.c_str(),
				name.c_str(),
				&m_ulOverlayHandle,
				&m_ulOverlayThumbnailHandle);
		if (overlayError != vr::VROverlayError_None)
		{
			if (overlayError == vr::VROverlayError_KeyInUse)
			{
				QMessageBox::critical(nullptr,
					"OpenVR Advanced Settings Overlay",
					"Another instance is already running.");
			}
			throw std::runtime_error(std::string(
				"Failed to create Overlay: "
				+ std::string(vr::VROverlay()->GetOverlayErrorNameFromEnum(
					overlayError))));
		}
		vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 3.7f); //2.5f
		vr::VROverlay()->SetOverlayInputMethod(
			m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse);
		vr::VROverlay()->SetOverlayFlag(
			m_ulOverlayHandle,
			vr::VROverlayFlags_SendVRSmoothScrollEvents,
			true);

		constexpr auto thumbiconFilename = "resources\\thumbicon.png";
		const auto thumbIconPath
			= paths::binaryDirectoryFindFile(thumbiconFilename);
		if (thumbIconPath.has_value())
		{
			vr::VROverlay()->SetOverlayFromFile(m_ulOverlayThumbnailHandle,
				thumbIconPath->c_str());
		}
		else
		{
			LOG(ERROR) << "Could not find the thumbnail file, \""
				<< thumbiconFilename << "\"";
		}

		// Too many render calls in too short time overwhelm Qt and an
		// assertion gets thrown. Therefore we use an timer to delay render
		// calls
		m_pRenderTimer.reset(new QTimer());
		m_pRenderTimer->setSingleShot(true);
		m_pRenderTimer->setInterval(5);
		connect(m_pRenderTimer.get(),
			SIGNAL(timeout()),
			this,
			SLOT(renderOverlay()));

		QOpenGLFramebufferObjectFormat fboFormat;
		fboFormat.setAttachment(
			QOpenGLFramebufferObject::CombinedDepthStencil);
		fboFormat.setTextureTarget(GL_TEXTURE_2D);
		m_pFbo.reset(new QOpenGLFramebufferObject(quickItem->size().toSize(), fboFormat));


		/* Setup the OpenGL rendering */
		/*******************************************/

		// Changed in Qt 6.0.0
		//m_window.setRenderTarget(m_pFbo.get());

		// Set graphics mode and context
		m_window.setGraphicsApi(QSGRendererInterface::OpenGL);
		m_window.setGraphicsDevice(QQuickGraphicsDevice::fromOpenGLContext(&m_openGLContext));

		// Set texture target
		m_window.setRenderTarget(QQuickRenderTarget::fromOpenGLTexture(m_pFbo->texture(), m_pFbo->size()));

		// Setup rendering redirecting
		quickItem->setParentItem(m_window.contentItem());
		m_window.setGeometry(0,
			0,
			static_cast<int>(quickItem->width()),
			static_cast<int>(quickItem->height()));

		// Changed in Qt 6.0.0
		//m_renderControl.initialize(&m_openGLContext);

		// Initialize and check for success
		if (!m_renderControl.initialize())
			LOG(ERROR) << "Failed to initialize redirected Qt Quick rendering!";
		
		/*******************************************/

		vr::HmdVector2_t vecWindowSize
			= {
				static_cast<float>(quickItem->width()),
				static_cast<float>(quickItem->height())
		};
		vr::VROverlay()->SetOverlayMouseScale(m_ulOverlayHandle,
			&vecWindowSize);

		connect(&m_renderControl,
			SIGNAL(renderRequested()),
			this,
			SLOT(OnRenderRequest()));
		connect(&m_renderControl,
			SIGNAL(sceneChanged()),
			this,
			SLOT(OnRenderRequest()));

		LOG(INFO) << "QQuickWindow flag setup for the overlay has started...";
		m_window.setFlags(Qt::FramelessWindowHint);
		m_window.setColor(QColorConstants::Transparent);
		LOG(INFO) << "QQuickWindow flag setup for the overlay completed!";
	}

	connect(&m_pumpEventsTimer,
		SIGNAL(timeout()),
		this,
		SLOT(OnTimeoutPumpEvents()));

	// Every 1ms we check if the current frame has advanced (for vsync)
	m_pumpEventsTimer.setInterval(1);
	m_pumpEventsTimer.start();
}

void OverlayController::OnRenderRequest()
{
	if (m_pRenderTimer && !m_pRenderTimer->isActive())
	{
		m_pRenderTimer->start();
	}
}

void OverlayController::renderOverlay()
{
	if (!m_desktopMode)
	{
		// skip rendering if the overlay isn't visible
		if (!vr::VROverlay()
			|| (!vr::VROverlay()->IsOverlayVisible(m_ulOverlayHandle)
				&& !vr::VROverlay()->IsOverlayVisible(
					m_ulOverlayThumbnailHandle)))
			return;

		// Polish texture items
		m_renderControl.polishItems();

		/* Specifies the end of a graphics frame. Calls to sync() or render()
		 * must be enclosed by calls to beginFrame() and endFrame().
		 * When this function is called, any graphics commands enqueued
		 * by the scenegraph are submitted to the context or command queue,
		 * whichever is applicable. */

		// New in Qt 6.0.0: begin the frame
		m_renderControl.beginFrame();

		m_renderControl.sync();
		m_renderControl.render();

		// New in Qt 6.0.0: end the frame
		m_renderControl.endFrame();

		GLuint unTexture = m_pFbo->texture();
		if (unTexture != 0)
		{
#if defined _WIN64 || defined _LP64
			// To avoid any compiler warning because of cast to a larger
			// pointer type (warning C4312 on VC)
			vr::Texture_t texture = {
				reinterpret_cast<void*>(
					static_cast<uint64_t>(unTexture)),
				vr::TextureType_OpenGL,
				vr::ColorSpace_Auto
			};
#else
			vr::Texture_t texture = { reinterpret_cast<void*>(unTexture),
									  vr::TextureType_OpenGL,
									  vr::ColorSpace_Auto };
#endif
			vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &texture);
		}
		m_openGLContext.functions()->glFlush(); // We need to flush otherwise
		// the texture may be empty.*/
	}
}

bool OverlayController::pollNextEvent(vr::VROverlayHandle_t ulOverlayHandle,
	vr::VREvent_t* pEvent)
{
	if (isDesktopMode())
	{
		return vr::VRSystem()->PollNextEvent(pEvent, sizeof(vr::VREvent_t));
	}
	return vr::VROverlay()->PollNextOverlayEvent(
		ulOverlayHandle, pEvent, sizeof(vr::VREvent_t));
}

QPoint OverlayController::getMousePositionForEvent(vr::VREvent_Mouse_t mouse)
{
	float y = mouse.y;
#ifdef __linux__
	float h = static_cast<float>(m_window.height());
	y = h - y;
#endif
	return QPoint(static_cast<int>(mouse.x), static_cast<int>(y));
}

// vsync implementation:
// this function triggers every 1ms
// this function should remain lightweight and only check if it's time to
// run mainEventLoop() or not.
void OverlayController::OnTimeoutPumpEvents()
{
	// get the current frame number from the VRSystem frame counter
	vr::VRSystem()->GetTimeSinceLastVsync(nullptr, &m_currentFrame);

	// Check if we are in the next frame yet
	if (m_currentFrame > m_lastFrame)
	{
		// If the frame has advanced since last check, it's time for our
		// main event loop. (this function should trigger about every
		// 11ms assuming 90fps compositor)
		mainEventLoop();
		updateRate.incrementCounter();

		// wait for the next frame after executing our main event loop
		// once.
		m_lastFrame = m_currentFrame;
		m_vsyncTooLateCounter = 0;
	}
	else if (m_vsyncTooLateCounter >= k_nonVsyncTickRate)
	{
		mainEventLoop();
		// m_lastFrame = m_currentFrame + 1 skips the next vsync frame
		// in case it was just about to trigger, to prevent double
		// updates faster than 11ms.
		m_lastFrame = m_currentFrame + 1;
		m_vsyncTooLateCounter = 0;
	}
	else
	{
		m_vsyncTooLateCounter++;
	}
}

void OverlayController::mainEventLoop()
{
	if (!vr::VRSystem())
		return;

	m_actions.UpdateStates();
	vr::VREvent_t vrEvent;
	bool chaperoneDataAlreadyUpdated = false;
	while (pollNextEvent(m_ulOverlayHandle, &vrEvent))
	{
		switch (vrEvent.eventType)
		{
		case vr::VREvent_MouseMove:
		{
			QPoint ptNewMouse = getMousePositionForEvent(vrEvent.data.mouse);
			if (ptNewMouse != m_ptLastMouse)
			{
				QMouseEvent mouseEvent(
					(QEvent::Type)QEvent::MouseMove,
					ptNewMouse,
					m_window.mapToGlobal(ptNewMouse),
					Qt::NoButton,
					m_lastMouseButtons,
					Qt::KeyboardModifier::NoModifier);

				// Changed in Qt 6.0.0
				/*QMouseEvent mouseEvent(QEvent::MouseMove,
									   ptNewMouse,
									   m_window.mapToGlobal(ptNewMouse),
									   Qt::NoButton,
									   m_lastMouseButtons,
									   nullptr);*/

				m_ptLastMouse = ptNewMouse;
				QCoreApplication::sendEvent(&m_window, &mouseEvent);
				OnRenderRequest();
			}
		}
		break;

		case vr::VREvent_MouseButtonDown:
		{
			QPoint ptNewMouse = getMousePositionForEvent(vrEvent.data.mouse);
			Qt::MouseButton button
				= vrEvent.data.mouse.button == vr::VRMouseButton_Right
				? Qt::RightButton
				: Qt::LeftButton;
			m_lastMouseButtons |= button;

			QMouseEvent mouseEvent(
				(QEvent::Type)QEvent::MouseButtonPress,
				ptNewMouse,
				m_window.mapToGlobal(ptNewMouse),
				button,
				m_lastMouseButtons,
				Qt::KeyboardModifier::NoModifier);

			// Changed in Qt 6.0.0
			/*QMouseEvent mouseEvent(QEvent::MouseButtonPress,
								   ptNewMouse,
								   m_window.mapToGlobal(ptNewMouse),
								   button,
								   m_lastMouseButtons,
								   nullptr);*/

			QCoreApplication::sendEvent(&m_window, &mouseEvent);
		}
		break;

		case vr::VREvent_MouseButtonUp:
		{
			QPoint ptNewMouse = getMousePositionForEvent(vrEvent.data.mouse);
			Qt::MouseButton button
				= vrEvent.data.mouse.button == vr::VRMouseButton_Right
				? Qt::RightButton
				: Qt::LeftButton;
			m_lastMouseButtons &= ~button;

			QMouseEvent mouseEvent(
				(QEvent::Type)QEvent::MouseButtonRelease,
				ptNewMouse,
				m_window.mapToGlobal(ptNewMouse),
				button,
				m_lastMouseButtons,
				Qt::KeyboardModifier::NoModifier);

			// Changed in Qt 6.0.0
			/*QMouseEvent mouseEvent(QEvent::MouseButtonRelease,
								   ptNewMouse,
								   m_window.mapToGlobal(ptNewMouse),
								   button,
								   m_lastMouseButtons,
								   nullptr);*/

			QCoreApplication::sendEvent(&m_window, &mouseEvent);
		}
		break;

		case vr::VREvent_ScrollSmooth:
		{
			// Wheel speed is defined as 1/8 of a degree
			QWheelEvent wheelEvent(
				m_ptLastMouse,
				m_window.mapToGlobal(m_ptLastMouse),
				QPoint(),
				QPoint(static_cast<int>(vrEvent.data.scroll.xdelta
					* (360.0f * 8.0f)),
					static_cast<int>(vrEvent.data.scroll.ydelta
						* (360.0f * 8.0f))),
				m_lastMouseButtons,
				Qt::KeyboardModifier::NoModifier,
				Qt::ScrollPhase::NoScrollPhase,
				false);

			// Changed in Qt 6.0.0
			/*QWheelEvent wheelEvent(
				m_ptLastMouse,
				m_window.mapToGlobal(m_ptLastMouse),
				QPoint(),
				QPoint(static_cast<int>(vrEvent.data.scroll.xdelta
					* (360.0f * 8.0f)),
					static_cast<int>(vrEvent.data.scroll.ydelta
						* (360.0f * 8.0f))),
				0,
				Qt::Vertical,
				m_lastMouseButtons,
				nullptr);*/
				
			QCoreApplication::sendEvent(&m_window, &wheelEvent);
		}
		break;

		case vr::VREvent_OverlayShown:
		{
			m_window.update();
		}
		break;

		case vr::VREvent_Quit:
		{
			LOG(INFO) << "Processing VREvent_Quit request...";
			vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some
			// time just in case

			exitApp();
			// Won't fallthrough, but also exitApp() wont, but QT won't
			// acknowledge
			exit(EXIT_SUCCESS);
		}

		case vr::VREvent_DashboardActivated:
		{
			LOG(INFO) << "Dashboard has been opened!";
			m_dashboardVisible = true;
		}
		break;

		case vr::VREvent_DashboardDeactivated:
		{
			LOG(INFO) << "Dashboard has been closed!";
			m_dashboardVisible = false;
		}
		break;

		case vr::VREvent_KeyboardDone:
		{
			char keyboardBuffer[1024];
			vr::VROverlay()->GetKeyboardText(keyboardBuffer, 1024);
			emit keyBoardInputSignal(QString(keyboardBuffer),
				static_cast<unsigned long>(
					vrEvent.data.keyboard.uUserValue));
		}
		break;

		case vr::VREvent_SeatedZeroPoseReset:
		{
			LOG(INFO) << "Seated position reset has been triggered!";
		}
		break;
		}
	}

	vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
	vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
		vr::TrackingUniverseStanding,
		0.0f,
		devicePoses,
		vr::k_unMaxTrackedDeviceCount);

	// HMD/Controller Velocities
	auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
		vr::TrackedControllerRole_LeftHand);
	float leftSpeed = 0.0f;
	if (leftId != vr::k_unTrackedDeviceIndexInvalid
		&& devicePoses[leftId].bPoseIsValid
		&& devicePoses[leftId].eTrackingResult
		== vr::TrackingResult_Running_OK)
	{
		auto& vel = devicePoses[leftId].vVelocity.v;
		leftSpeed
			= std::sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	}
	auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
		vr::TrackedControllerRole_RightHand);
	auto rightSpeed = 0.0f;
	if (rightId != vr::k_unTrackedDeviceIndexInvalid
		&& devicePoses[rightId].bPoseIsValid
		&& devicePoses[rightId].eTrackingResult
		== vr::TrackingResult_Running_OK)
	{
		auto& vel = devicePoses[rightId].vVelocity.v;
		rightSpeed
			= std::sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	}
	auto universe = vr::VRCompositor()->GetTrackingSpace();

	process.isOverlayVisible = vr::VROverlay()->IsOverlayVisible(m_ulOverlayHandle);

	if (m_ulOverlayThumbnailHandle != vr::k_ulOverlayHandleInvalid)
	{
		while (vr::VROverlay()->PollNextOverlayEvent(
			m_ulOverlayThumbnailHandle, &vrEvent, sizeof(vrEvent)))
		{
			switch (vrEvent.eventType)
			{
			case vr::VREvent_OverlayShown:
			{
				m_window.update();
			}
			break;
			}
		}
	}
}

QString OverlayController::getVersionString()
{
	return QString(application_strings::applicationVersionString);
}

QUrl OverlayController::getVRRuntimePathUrl()
{
	return m_runtimePathUrl;
}

bool OverlayController::soundDisabled()
{
	return m_noSound;
}

const vr::VROverlayHandle_t& OverlayController::overlayHandle()
{
	return m_ulOverlayHandle;
}

const vr::VROverlayHandle_t& OverlayController::overlayThumbnailHandle()
{
	return m_ulOverlayThumbnailHandle;
}

void OverlayController::showKeyboard(QString existingText,
	unsigned long userValue)
{
	vr::VROverlay()->ShowKeyboardForOverlay(
		m_ulOverlayHandle,
		vr::k_EGamepadTextInputModeNormal,
		vr::k_EGamepadTextInputLineModeSingleLine,
		0,
		"KinectToVR Overlay",
		1024,
		existingText.toStdString().c_str(),
		userValue);
	setKeyboardPos();
}

void OverlayController::setKeyboardPos()
{
	vr::HmdVector2_t emptyvec;
	emptyvec.v[0] = 0;
	emptyvec.v[1] = 0;
	vr::HmdRect2_t empty;
	empty.vTopLeft = emptyvec;
	empty.vBottomRight = emptyvec;
	vr::VROverlay()->SetKeyboardPositionForOverlay(m_ulOverlayHandle, empty);
}

void OverlayController::checkNewVersion()
{
	std::string version_data;
	try
	{
		curlpp::Cleanup myCleanup;
		std::ostringstream os;
		os << curlpp::options::Url(application_strings::versionCheckUrl);
		version_data = os.str();
	}
	catch (curlpp::RuntimeError& e)
	{
		LOG(ERROR) << "Version checking failed: Runtime connection error: " << e.what();
	}
	catch (curlpp::LogicError& e)
	{
		LOG(ERROR) << "Version checking failed: Logic connection error: " << e.what();
	}
	
	if (!version_data.empty())
	{
		QByteArray replyByteData = QByteArray(version_data.c_str(), version_data.length());
		LOG(INFO) << "Version checking: Recieved data: " << replyByteData;
		
		QJsonParseError parseError;
		m_remoteVersionJsonDocument
			= QJsonDocument::fromJson(replyByteData, &parseError);
		if (parseError.error == QJsonParseError::NoError)
		{
			m_remoteVersionJsonObject = m_remoteVersionJsonDocument.object();
			m_remoteVersionMajor
				= m_remoteVersionJsonObject.value("major").toInt();
			m_remoteVersionMinor
				= m_remoteVersionJsonObject.value("minor").toInt();
			m_remoteVersionPatch
				= m_remoteVersionJsonObject.value("patch").toInt();
			m_updateMessage
				= m_remoteVersionJsonObject.value("message").toString();

			// this is a little convoluted to ensure if our local version is
			// somehow higher than remote, it doesn't detect an update from
			// just higher remote "minor" or "patch" values.
			if ((m_remoteVersionMajor > m_localVersionMajor)
				|| (m_remoteVersionMajor == m_localVersionMajor
					&& m_remoteVersionMinor > m_localVersionMinor)
				|| (m_remoteVersionMajor == m_localVersionMajor
					&& m_remoteVersionMinor == m_localVersionMinor
					&& m_remoteVersionPatch > m_localVersionPatch))
			{
				m_newVersionDetected = true;
				if (m_updateMessage.length() > 0)
				{
					m_versionCheckText = m_updateMessage;
				}
				else
				{
					m_versionCheckText =
						"Newer version ("
						+ QString::number(m_remoteVersionMajor) + "."
						+ QString::number(m_remoteVersionMinor) + "."
						+ QString::number(m_remoteVersionPatch)
						+ ") available.";
				}
				LOG(INFO)
					<< "Version Check: Newer version ( " << m_remoteVersionMajor
					<< "." << m_remoteVersionMinor << "."
					<< m_remoteVersionPatch << " ) available.";
				LOG(INFO)
					<< "Version Check: Newer version ( " << m_updateMessage.toStdString() << " ) available.";
			}
			else
			{
				m_newVersionDetected = false;
				LOG(INFO) << "Currently installed version is the latest.";
			}
		}
		else
		{
			LOG(ERROR)
				<< "Version checking failed: Global parsing error: " << parseError.error;
		}
	}
	else
	{
		LOG(ERROR) << "Version checking failed: Global connection error, request empty!";
	}
}

//void OverlayController::playActivationSound()
//{
//	if (!m_noSound)
//	{
//		m_activationSoundEffect.play();
//	}
//}
//
//void OverlayController::playFocusChangedSound()
//{
//	if (!m_noSound)
//	{
//		m_focusChangedSoundEffect.play();
//	}
//}
//
//void OverlayController::playAlarm01Sound(bool loop)
//{
//	if (!m_noSound && !m_alarm01SoundEffect.isPlaying())
//	{
//		if (loop)
//		{
//			m_alarm01SoundEffect.setLoopCount(QSoundEffect::Infinite);
//		}
//		else
//		{
//			m_alarm01SoundEffect.setLoopCount(1);
//		}
//		m_alarm01SoundEffect.play();
//	}
//}
//
//void OverlayController::setAlarm01SoundVolume(float vol)
//{
//	m_alarm01SoundEffect.setVolume(static_cast<double>(vol));
//}
//
//void OverlayController::cancelAlarm01Sound()
//{
//	m_alarm01SoundEffect.stop();
//}

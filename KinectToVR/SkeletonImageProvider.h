#pragma once
#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include "runtimeConfig.h"
#include "TrackingDeviceBase.h"

// Select a skeleton drawing method
#define SKELETON_PROJECTED
//#define SKELETON_CENTERED

// We're reducing render by 3 times to lower CPU usage
constexpr int s_reduce = 3, // By how much will we reduce image (qml:1542)
	s_mat_width = 2200 / s_reduce,
	s_mat_height = 2068 / s_reduce;

// Where to scale by 1.0 in perspective
constexpr double s_normal_distance = 3;

// Draw a skeleton line
inline void s_line(cv::Mat& input, ktvr::ITrackedJointType from, ktvr::ITrackedJointType to)
{
    // Choose a color for the line if joint isn't tracked properly
    const cv::Scalar s_color = 
		process.trackingStates[from] != ktvr::State_Tracked || process.trackingStates[to] != ktvr::State_Tracked ?
		cv::Scalar(255, 189, 0) : cv::Scalar(255, 255, 255);

    // Create a line in ortho mode
	cv::Point p_from = cv::Point(
		process.jointPositions[from].x * 1000 / s_reduce,
		process.jointPositions[from].y * -1000 / s_reduce),
		p_to = cv::Point(
			process.jointPositions[to].x * 1000 / s_reduce,
			process.jointPositions[to].y * -1000 / s_reduce);

#if defined(SKELETON_PROJECTED)
	// Compose perspective constants, make it 70%
    const double s_from_multiply = .7 * (s_normal_distance / process.jointPositions[from].z),
        s_to_multiply = .7 * (s_normal_distance / process.jointPositions[to].z);
	
    // Draw normally, in "perspective" projection
    p_from.x *= s_from_multiply;
    p_from.y *= s_from_multiply;
	
    p_to.x *= s_to_multiply;
    p_to.y *= s_to_multiply;

#elif defined(SKELETON_CENTERED)
	// Center all the drawing to hips
    p_from.x -= process.jointPositions[Joint_SpineMiddle].x * 1000 / s_reduce;
    p_from.y -= process.jointPositions[Joint_SpineMiddle].y * 1000 / s_reduce;

    p_to.x -= process.jointPositions[Joint_SpineMiddle].x * 1000 / s_reduce;
    p_to.y -= process.jointPositions[Joint_SpineMiddle].y * 1000 / s_reduce;
	
#endif

	// Offset line to center
    p_from.x += s_mat_width / 2;
    p_to.x += s_mat_width / 2;

    p_from.y += 810 / s_reduce;
    p_to.y += 810 / s_reduce;

	// Draw the line
	cv::line(input, p_from, p_to,
        s_color, 9 / s_reduce);
}

class SkeletonImageProvider : public QQuickImageProvider
{
public:
    SkeletonImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override
    {
    	// Create skeleton mat
        cv::Mat skeletonMat(s_mat_height, s_mat_width, CV_8UC3);
    	
    	// Fill with black
    	skeletonMat.setTo(cv::Scalar(0, 0, 0));

		// Draw the skeleton with from-to lines
    	// Head
        s_line(skeletonMat, ktvr::Joint_Head, ktvr::Joint_Neck);
        s_line(skeletonMat, ktvr::Joint_Neck, ktvr::Joint_SpineShoulder);

    	// Upper left limb
        s_line(skeletonMat, ktvr::Joint_SpineShoulder, ktvr::Joint_ShoulderLeft);
        s_line(skeletonMat, ktvr::Joint_ShoulderLeft, ktvr::Joint_ElbowLeft);
        s_line(skeletonMat, ktvr::Joint_ElbowLeft, ktvr::Joint_WristLeft);
        s_line(skeletonMat, ktvr::Joint_WristLeft, ktvr::Joint_HandLeft);
        s_line(skeletonMat, ktvr::Joint_HandLeft, ktvr::Joint_HandTipLeft);
        s_line(skeletonMat, ktvr::Joint_HandLeft, ktvr::Joint_ThumbLeft);

    	// Upper right limb
        s_line(skeletonMat, ktvr::Joint_SpineShoulder, ktvr::Joint_ShoulderRight);
        s_line(skeletonMat, ktvr::Joint_ShoulderRight, ktvr::Joint_ElbowRight);
        s_line(skeletonMat, ktvr::Joint_ElbowRight, ktvr::Joint_WristRight);
        s_line(skeletonMat, ktvr::Joint_WristRight, ktvr::Joint_HandRight);
        s_line(skeletonMat, ktvr::Joint_HandRight, ktvr::Joint_HandTipRight);
        s_line(skeletonMat, ktvr::Joint_HandRight, ktvr::Joint_ThumbRight);

    	// Spine
        s_line(skeletonMat, ktvr::Joint_SpineShoulder, ktvr::Joint_SpineMiddle);
        s_line(skeletonMat, ktvr::Joint_SpineMiddle, ktvr::Joint_SpineWaist);

    	// Lower left limb
        s_line(skeletonMat, ktvr::Joint_SpineWaist, ktvr::Joint_HipLeft);
        s_line(skeletonMat, ktvr::Joint_HipLeft, ktvr::Joint_KneeLeft);
        s_line(skeletonMat, ktvr::Joint_KneeLeft, ktvr::Joint_AnkleLeft);
        s_line(skeletonMat, ktvr::Joint_AnkleLeft, ktvr::Joint_FootLeft);

        // Lower right limb
        s_line(skeletonMat, ktvr::Joint_SpineWaist, ktvr::Joint_HipRight);
        s_line(skeletonMat, ktvr::Joint_HipRight, ktvr::Joint_KneeRight);
        s_line(skeletonMat, ktvr::Joint_KneeRight, ktvr::Joint_AnkleRight);
        s_line(skeletonMat, ktvr::Joint_AnkleRight, ktvr::Joint_FootRight);
        
		// Return a composed pixel map
		return QPixmap::fromImage(
			// Convert cv::Mat to QImage
			QImage(skeletonMat.data,
				skeletonMat.cols,
				skeletonMat.rows,
				static_cast<int>(skeletonMat.step),
				QImage::Format_RGB888)); // 8UC3 format
    }
};


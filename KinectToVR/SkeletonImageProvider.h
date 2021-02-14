#pragma once
#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>
#include <opencv/cv.hpp>
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
inline void s_line(cv::Mat& input, ITrackedJointType from, ITrackedJointType to)
{
    // Choose a color for the line if joint isn't tracked properly
    const cv::Scalar s_color = 
		process.trackingStates[from] != State_Tracked || process.trackingStates[to] != State_Tracked ?
		cv::Scalar(255, 189, 0) : cv::Scalar(255, 255, 255);

	cv::Point p_from = cv::Point(
		process.jointPositions[from].x * 1000 / s_reduce,
		process.jointPositions[from].y * -1000 / s_reduce),
		p_to = cv::Point(
			process.jointPositions[to].x * 1000 / s_reduce,
			process.jointPositions[to].y * -1000 / s_reduce);

	//Draw a line from - to a skeleton joint
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
	// Draw normally, in orthographic projection

	// Offset to center
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
        s_line(skeletonMat, Joint_Head, Joint_Neck);
        s_line(skeletonMat, Joint_Neck, Joint_SpineShoulder);

    	// Upper left limb
        s_line(skeletonMat, Joint_SpineShoulder, Joint_ShoulderLeft);
        s_line(skeletonMat, Joint_ShoulderLeft, Joint_ElbowLeft);
        s_line(skeletonMat, Joint_ElbowLeft, Joint_WristLeft);
        s_line(skeletonMat, Joint_WristLeft, Joint_HandLeft);
        s_line(skeletonMat, Joint_HandLeft, Joint_HandTipLeft);
        s_line(skeletonMat, Joint_HandLeft, Joint_ThumbLeft);

    	// Upper right limb
        s_line(skeletonMat, Joint_SpineShoulder, Joint_ShoulderRight);
        s_line(skeletonMat, Joint_ShoulderRight, Joint_ElbowRight);
        s_line(skeletonMat, Joint_ElbowRight, Joint_WristRight);
        s_line(skeletonMat, Joint_WristRight, Joint_HandRight);
        s_line(skeletonMat, Joint_HandRight, Joint_HandTipRight);
        s_line(skeletonMat, Joint_HandRight, Joint_ThumbRight);

    	// Spine
        s_line(skeletonMat, Joint_SpineShoulder, Joint_SpineMiddle);
        s_line(skeletonMat, Joint_SpineMiddle, Joint_SpineWaist);

    	// Lower left limb
        s_line(skeletonMat, Joint_SpineWaist, Joint_HipLeft);
        s_line(skeletonMat, Joint_HipLeft, Joint_KneeLeft);
        s_line(skeletonMat, Joint_KneeLeft, Joint_AnkleLeft);
        s_line(skeletonMat, Joint_AnkleLeft, Joint_FootLeft);

        // Lower right limb
        s_line(skeletonMat, Joint_SpineWaist, Joint_HipRight);
        s_line(skeletonMat, Joint_HipRight, Joint_KneeRight);
        s_line(skeletonMat, Joint_KneeRight, Joint_AnkleRight);
        s_line(skeletonMat, Joint_AnkleRight, Joint_FootRight);
        
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


#pragma once
#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>
#include <opencv/cv.hpp>
#include "runtimeConfig.h"
#include "TrackingDeviceBase.h"

//TODO: somehow make image smaller to reduce %CPU
constexpr int s_mat_width = 2028;
constexpr int  s_mat_height = 2068;

inline void s_line(cv::Mat& input, ITrackedJointType from, ITrackedJointType to)
{
	cv::line(input,
		cv::Point(
			process.jointPositions[from].x * 1000 + s_mat_width / 2,
			process.jointPositions[from].y * -1000 + 810),
		cv::Point(
			process.jointPositions[to].x * 1000 + s_mat_width / 2,
			process.jointPositions[to].y * -1000 + 810),
		cv::Scalar(255, 255, 255), 15);
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
        cv::Mat skeletonMat(s_mat_height, s_mat_width, CV_8UC1);

		// Draw skeleton
        s_line(skeletonMat, Joint_Head, Joint_Neck);
        s_line(skeletonMat, Joint_Neck, Joint_SpineShoulder);
    	
        s_line(skeletonMat, Joint_SpineShoulder, Joint_ShoulderLeft);
        s_line(skeletonMat, Joint_ShoulderLeft, Joint_ElbowLeft);
        s_line(skeletonMat, Joint_ElbowLeft, Joint_WristLeft);
        s_line(skeletonMat, Joint_WristLeft, Joint_HandLeft);
        s_line(skeletonMat, Joint_HandLeft, Joint_HandTipLeft);
        s_line(skeletonMat, Joint_HandLeft, Joint_ThumbLeft);

        s_line(skeletonMat, Joint_SpineShoulder, Joint_ShoulderRight);
        s_line(skeletonMat, Joint_ShoulderRight, Joint_ElbowRight);
        s_line(skeletonMat, Joint_ElbowRight, Joint_WristRight);
        s_line(skeletonMat, Joint_WristRight, Joint_HandRight);
        s_line(skeletonMat, Joint_HandRight, Joint_HandTipRight);
        s_line(skeletonMat, Joint_HandRight, Joint_ThumbRight);
    	
        s_line(skeletonMat, Joint_SpineShoulder, Joint_SpineMiddle);
        s_line(skeletonMat, Joint_SpineMiddle, Joint_SpineWaist);
    	
        s_line(skeletonMat, Joint_SpineWaist, Joint_HipLeft);
        s_line(skeletonMat, Joint_HipLeft, Joint_KneeLeft);
        s_line(skeletonMat, Joint_KneeLeft, Joint_AnkleLeft);
        s_line(skeletonMat, Joint_AnkleLeft, Joint_FootLeft);

        s_line(skeletonMat, Joint_SpineWaist, Joint_HipRight);
        s_line(skeletonMat, Joint_HipRight, Joint_KneeRight);
        s_line(skeletonMat, Joint_KneeRight, Joint_AnkleRight);
        s_line(skeletonMat, Joint_AnkleRight, Joint_FootRight);
        
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
            colorTable.push_back(qRgb(i, i, i));
    	
        // Copy input Mat
        const uchar* qImageBuffer = (const uchar*)skeletonMat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, skeletonMat.cols, skeletonMat.rows, skeletonMat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        
        return QPixmap::fromImage(img);
    }
};


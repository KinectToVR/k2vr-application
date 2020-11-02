#ifndef BUTTONHANDLERS_H
#define BUTTONHANDLERS_H

#include <QObject>
#include <QQuickItem>
#include <Windows.h>
#include <iostream>
#include <sstream>

/* For priniting debug info to VS */
#define VSDebug( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << '\n';                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

/* Class for handling and processing signals from QML */
class signalHandler : public QObject
{
Q_OBJECT
public:
	explicit signalHandler(QObject* parent = nullptr);

signals:

	/* one for click and two for click with arguments */
public slots:
	void cppSlot(const QString& msg);
	void multiCpp(const QString& msg, const QString& arg);
};

#endif

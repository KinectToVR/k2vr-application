#include <buttonHandlers.h>


signalHandler::signalHandler(QObject* parent) : QObject(parent)
{

}

/* Process button click, like claibrate, showoffsets, etc. */
void signalHandler::cppSlot(const QString& msg)
{
    std::string str = "Called the C++ slot with message: " + msg.toStdString() + '\n';
    VSDebug(str.c_str());
}

/* Process filter change of button click with extra arguments */
void signalHandler::multiCpp(const QString& msg, const QString& arg) 
{
    std::string str = "Called the C++ slot with message: " + msg.toStdString() + ", with arguments: " + arg.toStdString() + '\n';
    VSDebug(str.c_str());
}
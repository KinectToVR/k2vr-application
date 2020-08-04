#include <Logging.h>

const char* logConfigFileName = "logging.conf";

const char* logConfigDefault =
"* GLOBAL:\n"
u8"	FORMAT = \"[%level] %datetime{%Y年%M月%d日 %H時%m分%s}秒: %msg\"\n"
"	FILENAME = \"KinectToVR_EX.log\"\n"
"	ENABLED = true\n"
"	TO_FILE = true\n"
"	TO_STANDARD_OUTPUT = true\n"
"	MAX_LOG_FILE_SIZE = 2097152 ## 2MB\n"
"* TRACE:\n"
"	ENABLED = false\n"
"* DEBUG:\n"
"	ENABLED = false\n";

/* Configure easylogging and print first message */
KINECTTOVR_LIB void initLogging() {
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Configurations conf(logConfigFileName);
    conf.parseFromText(logConfigDefault);
    //conf.parseFromFile(logConfigFileName);
    conf.setRemainingToDefault();
    el::Loggers::reconfigureAllLoggers(conf);

    LOG(INFO) << u8"～～～KinectToVR EXのロギングセッションはここからスタート！～～～";
}

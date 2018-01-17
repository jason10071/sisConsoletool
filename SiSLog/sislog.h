#ifndef SISLOG_H
#define SISLOG_H

#include <stdio.h>
#include <string>
#include <map>
#include "sislogdllexportdefine.h"

/* Log Type */
#define LOG_TYPE_NONE     0x1
#define LOG_TYPE_LOGCAT   0x2
#define LOG_TYPE_FILE     0x4

/* Level */
#define LEVEL_DEBUG     "D"
#define LEVEL_INFO      "I"
#define LEVEL_WARNING   "W"
#define LEVEL_ERROR     "E"

/* log instance owner */
#define OWNER_SIS_NAME  "SiS"

/* symbol*/
#define MESSAGE_BEGIN ":~$ "

namespace SiS
{

class SISLOG_EXPORT SiSLog
{
public:
    enum LogLabel
    {
        LABEL_TIMESTAMP = 0,
        LABEL_THREAD_ID,
        LABEL_LEVEL,
        LABEL_TAG,
        LABEL_FUNCTION,
        LABEL_FILE,
        LABEL_LINE,
        LABEL_OWNER,
        LABEL_COUNT, // label count, also express none
    };

    enum LogFormat
    {
        FORMATE_SIMPLE = 0,
        FORMATE_DETAIL,
    };

    void releaseLogFile();

    static SiS::SiSLog* getOwnerSiS();

    static SiSLog* getInstance(std::string owner);
    static void freeInstance(SiSLog* sisLog);

    void setLogFormat(LogFormat logFormat);

    void setLogFormat(LogLabel o0 = LABEL_COUNT,
                      LogLabel o1 = LABEL_COUNT,
                      LogLabel o2 = LABEL_COUNT,
                      LogLabel o3 = LABEL_COUNT,
                      LogLabel o4 = LABEL_COUNT,
                      LogLabel o5 = LABEL_COUNT,
                      LogLabel o6 = LABEL_COUNT,
                      LogLabel o7 = LABEL_COUNT);

    void setLogType(int logType);
    void setLogFile(const char* logFile);

    void setDebugVisible(bool debugVisible);
    void setInfoVisible(bool infoVisible);
    void setWarningVisible(bool warningVisible);
    void setErrorVisible(bool errorVisible);

    bool isLOG_D();
    bool isLOG_I();
    bool isLOG_W();
    bool isLOG_E();

    void printLogFormat();
    void printLog(const char* level, const char* tag, const char* function, const char* file, const int line, const char* message, ...);

private:
    SiSLog(std::string owner);
    ~SiSLog();

    static std::map<std::string, SiSLog*> & createInstanceMap();
    void init();
    const std::string getLogLabelStr(LogLabel logLabel);
    const std::string getCurrentTimeString();
    const std::string getCurrentThreadIDString();

    void appendLabelTimeStamp(char* fmt);
    void appendLabelThreadID(char* fmt);
    void appendLabelLevel(char* fmt, const char* level);
    void appendLabelTag(char* fmt, const char* tag);
    void appendLabelFunction(char* fmt, const char* function);
    void appendLabelFile(char* fmt, const char* file);
    void appendLabelLine(char* fmt, const int line);
    void appendLabelOwner(char* fmt);

private:
    static std::map<std::string, SiSLog*> *m_pInstances; /* <owner, sisLog> */
    static SiS::SiSLog* m_ownerSiS;
    std::string m_owner;

    int m_logType;
    FILE* m_logFile;

    bool m_debugVisible;
    bool m_infoVisible;
    bool m_warningVisible;
    bool m_errorVisible;

    LogLabel m_logLabelOrder[LABEL_COUNT];
};

} // SiS

#define SIS_LOG_D(sisLogOwner, tag, message, ...) if(sisLogOwner != 0 && sisLogOwner->isLOG_D()) \
    sisLogOwner->printLog(LEVEL_DEBUG,   tag, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__)

#define SIS_LOG_I(sisLogOwner, tag, message, ...) if(sisLogOwner != 0 && sisLogOwner->isLOG_I()) \
    sisLogOwner->printLog(LEVEL_INFO,    tag, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__)

#define SIS_LOG_W(sisLogOwner, tag, message, ...) if(sisLogOwner != 0 && sisLogOwner->isLOG_W()) \
    sisLogOwner->printLog(LEVEL_WARNING, tag, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__)

#define SIS_LOG_E(sisLogOwner, tag, message, ...) if(sisLogOwner != 0 && sisLogOwner->isLOG_E()) \
    sisLogOwner->printLog(LEVEL_ERROR,   tag, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__)

#endif // SISLOG_H

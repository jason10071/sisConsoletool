#include "sislog.h"

#include <stdarg.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include "Windows.h"
#else
#include <sys/time.h>
//#include <thread>
//#include <mutex>
//static std::mutex g_mutex_sislog;
#endif

using namespace SiS;

#define TAG "SiSLog"

std::map<std::string, SiSLog*> * SiS::SiSLog::m_pInstances = 0;
SiS::SiSLog* SiS::SiSLog::m_ownerSiS = 0;

#define FORMAT_MAX_LENGTH 1024

SiSLog::SiSLog(std::string owner) :
    m_owner(owner),
    m_logType(LOG_TYPE_LOGCAT),
    m_logFile(0),
    m_debugVisible(false),
    m_infoVisible(true),
    m_warningVisible(true),
    m_errorVisible(true)
{
    init();
}

SiSLog::~SiSLog()
{
    // don't delete somethings is non-static
}

void
SiSLog::releaseLogFile()
{
    if( m_logFile != 0 )
    {
        SIS_LOG_D( this, TAG, "release log file : (%d)", m_logFile );

        fclose(m_logFile);
        m_logFile = 0;
    }
}

std::map<std::string, SiSLog*> &
SiSLog::createInstanceMap()
{
    static std::map<std::string, SiSLog*> instanceMap;
    return instanceMap;
}

SiS::SiSLog*
SiSLog::getOwnerSiS()
{
    return SiS::SiSLog::m_ownerSiS == 0 ? SiS::SiSLog::getInstance(OWNER_SIS_NAME) : SiS::SiSLog::m_ownerSiS;
}

SiSLog*
SiSLog::getInstance(std::string owner)
{
#ifdef WIN32
    // to do
#else
    //std::lock_guard<std::mutex> guard(g_mutex_sislog);
#endif

//    int cnt = 0;
//    while(cnt < 10)
//    {
//        printf("owner=%s\n", owner.c_str()); // temp printf
//        usleep(1000 * 1000);
//        cnt++;
//    }

    if( m_pInstances == 0 )
    {
        m_pInstances = &createInstanceMap();
    }

    SiSLog* pInstance = 0;

    /* find if has instanced */
    std::map<std::string, SiSLog*>::iterator it = m_pInstances->find( owner );
    if ( it != m_pInstances->end() )
    {
        pInstance = it->second; // has instanced
    }
    else
    {
        pInstance = new SiSLog(owner); // has not instanced, new
        m_pInstances->insert( std::pair<std::string, SiSLog*>(owner, pInstance) );

        /* cache m_ownerSiS */
        if( owner.compare(OWNER_SIS_NAME) == 0 )
        {
            SiS::SiSLog::m_ownerSiS = pInstance;
        }
    }

//    printf("m_pInstances->size()=%d\n", m_pInstances->size()); // temp printf
    return pInstance;
}

void
SiSLog::freeInstance(SiSLog* sisLog)
{
    std::string owner;

    /* find owner if exist */
    for(std::map<std::string, SiSLog*>::iterator it = m_pInstances->begin(); it != m_pInstances->end(); ++it)
    {
        if( sisLog == it->second )
        {
            owner = it->first;
            break;
        }
    }

    if( !owner.empty() )
    {
//        printf("freeInstance(%s)\n", owner.c_str()); // temp printf
        m_pInstances->erase(owner);
        delete sisLog;
    }
}

void
SiSLog::init()
{
    /* default Log Format */
    setLogFormat(FORMATE_SIMPLE);
}

const std::string
SiSLog::getLogLabelStr(LogLabel logLabel)
{
    switch (logLabel)
    {
    case LABEL_TIMESTAMP:
        return "Timestamp";
        break;
    case LABEL_THREAD_ID:
        return "ThreadID";
        break;
    case LABEL_LEVEL:
        return "Level";
        break;
    case LABEL_TAG:
        return "Tag";
        break;
    case LABEL_FUNCTION:
        return "Function";
        break;
    case LABEL_FILE:
        return "File";
        break;
    case LABEL_LINE:
        return "Line";
        break;
    case LABEL_OWNER:
        return "Owner";
        break;
    case LABEL_COUNT:
    default:
        return "Unknow";
        break;
    }
}

const std::string
SiSLog::getCurrentTimeString()
{
#ifdef WIN32
    SYSTEMTIME lt;
    GetLocalTime(&lt);

    char currentTime[128] = {0};
    sprintf(currentTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            lt.wYear, lt.wMonth, lt.wDay,
            lt.wHour, lt.wMinute, lt.wSecond,
            lt.wMilliseconds);

    return std::string(currentTime);
#else
    char            fmt[64], buf[64];
    struct timeval  tv;
    struct tm       *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    strftime(fmt, sizeof fmt, "%Y-%m-%d %H:%M:%S.%%06u", tm);
    snprintf(buf, sizeof buf, fmt, tv.tv_usec);
    return std::string(buf);
#endif
}

const std::string
SiSLog::getCurrentThreadIDString()
{
/*
#ifdef WIN32
    char threadID[128] = {0};
    sprintf(threadID, "%d", GetCurrentThreadId());

    return std::string(threadID);
#else
  //printf("PID of this process: %d\n", getpid());
  //printf("The ID of this thread is: %u\n", (unsigned int)pthread_self());

    char threadID[128] = {0};
    sprintf(threadID, "%lu", (unsigned int)pthread_self());

    return std::string(threadID);
#endif
*/
	return "";
}

void
SiSLog::setLogFormat(LogFormat logFormat)
{
    switch (logFormat) {
    case FORMATE_SIMPLE:
        setLogFormat(
                    SiSLog::LABEL_LEVEL
                    );
        break;
    case FORMATE_DETAIL:
        setLogFormat(
                    LABEL_TIMESTAMP,
                    //LABEL_THREAD_ID,
                    LABEL_LEVEL,
                    LABEL_TAG
                    //LABEL_FUNCTION,
                    //LABEL_FILE,
                    //LABEL_LINE,
                    //LABEL_OWNER
                    );
        break;
    default:
        break;
    }
}

void
SiSLog::setLogFormat(LogLabel o0, LogLabel o1, LogLabel o2, LogLabel o3,
                     LogLabel o4, LogLabel o5, LogLabel o6, LogLabel o7)
{
    /* Note : index < LABEL_COUNT */
    m_logLabelOrder[0] = o0;
    m_logLabelOrder[1] = o1;
    m_logLabelOrder[2] = o2;
    m_logLabelOrder[3] = o3;
    m_logLabelOrder[4] = o4;
    m_logLabelOrder[5] = o5;
    m_logLabelOrder[6] = o6;
    m_logLabelOrder[7] = o7;
}

void
SiSLog::setLogType(int logType)
{
    m_logType = logType;
}

void
SiSLog::setLogFile(const char* logFile)
{
    releaseLogFile();

    m_logFile = fopen( logFile, "w+" );

    if ( !m_logFile )
    {
        SIS_LOG_E( this, TAG, "Set/Open log file fail: \"%s\"", logFile );
    }
    else
    {
        SIS_LOG_D( this, TAG, "Set/Open log file succeed: \"%s\" (%d)", logFile, m_logFile );
    }
}

void
SiSLog::setDebugVisible(bool debugVisible)
{
    m_debugVisible = debugVisible;
}

void
SiSLog::setInfoVisible(bool infoVisible)
{
    m_infoVisible = infoVisible;
}

void
SiSLog::setWarningVisible(bool warningVisible)
{
    m_warningVisible = warningVisible;
}

void
SiSLog::setErrorVisible(bool errorVisible)
{
    m_errorVisible = errorVisible;
}

bool
SiSLog::isLOG_D()
{
    return m_debugVisible;
}

bool
SiSLog::isLOG_I()
{
    return m_infoVisible;
}

bool
SiSLog::isLOG_W()
{
    return m_warningVisible;
}

bool
SiSLog::isLOG_E()
{
    return m_errorVisible;
}

void
SiSLog::printLogFormat()
{
    char fmt[FORMAT_MAX_LENGTH] = {0};

    /* label */
    for( int order = LABEL_TIMESTAMP; order < LABEL_COUNT; order++ )
    {
        switch (m_logLabelOrder[order])
        {
        case LABEL_TIMESTAMP:
        case LABEL_THREAD_ID:
        case LABEL_LEVEL:
        case LABEL_TAG:
        case LABEL_FUNCTION:
        case LABEL_FILE:
        case LABEL_LINE:
        case LABEL_OWNER:
            strcat(fmt, "[");
            strcat(fmt, getLogLabelStr(m_logLabelOrder[order]).c_str());
            strcat(fmt, "]");
            break;
        case LABEL_COUNT:
        default:
            break;
        }
    }

    /* message */
    strcat(fmt, MESSAGE_BEGIN);
    strcat(fmt, "Message\n");

    /* print */
    if( m_logType & LOG_TYPE_LOGCAT )
    {
        printf(fmt);
        fflush(stdout);
    }

    if( m_logFile && (m_logType & LOG_TYPE_FILE) )
    {
        fprintf(m_logFile, fmt);
        fflush(m_logFile);
    }
}

void
SiSLog::printLog(const char* level, const char* tag, const char* function, const char* file, const int line, const char* message, ...)
{
    if( m_logType & LOG_TYPE_NONE )
    {
        return;
    }

    /* prepare format */
    const int len = strlen(message) + FORMAT_MAX_LENGTH;
    char* fmt = new char[len];
    memset(fmt, 0, len * sizeof(char));

    /* append label */
    for( int order = LABEL_TIMESTAMP; order < LABEL_COUNT; order++ )
    {
        switch (m_logLabelOrder[order])
        {
        case LABEL_TIMESTAMP:
            appendLabelTimeStamp(fmt);
            break;
        case LABEL_THREAD_ID:
            appendLabelThreadID(fmt);
            break;
        case LABEL_LEVEL:
            appendLabelLevel(fmt, level);
            break;
        case LABEL_TAG:
            appendLabelTag(fmt, tag);
            break;
        case LABEL_FUNCTION:
            appendLabelFunction(fmt, function);
            break;
        case LABEL_FILE:
            appendLabelFile(fmt, file);
            break;
        case LABEL_LINE:
            appendLabelLine(fmt, line);
            break;
        case LABEL_OWNER:
            appendLabelOwner(fmt);
            break;
        case LABEL_COUNT:
        default:
            break;
        }
    }

    /* append message */
    strcat(fmt, MESSAGE_BEGIN);
    strcat(fmt, message);
    strcat(fmt, "\n");

//    printf("m_logFile=%d, m_logType=%d\n", m_logFile, m_logType); // temp printf

    /* print */
    if( m_logType & LOG_TYPE_LOGCAT )
    {
        va_list arglist;
        va_start( arglist, message );
        vprintf(fmt, arglist);
        va_end( arglist );

        fflush(stdout);
    }

    if( m_logFile && (m_logType & LOG_TYPE_FILE) )
    {
        va_list arglist;
        va_start( arglist, message );
        vfprintf(m_logFile, fmt, arglist); // vfprintf may cause crash : arg_ptr somethings error !
        va_end( arglist );

        fflush(m_logFile);
    }

    delete [] fmt;
}

void
SiSLog::appendLabelTimeStamp(char* fmt)
{
    std::string timestamp = getCurrentTimeString();

    strcat(fmt, "[");
    strcat(fmt, timestamp.c_str());
    strcat(fmt, "]");
}

void
SiSLog::appendLabelThreadID(char* fmt)
{
    std::string threadID = getCurrentThreadIDString();

    strcat(fmt, "[");
    strcat(fmt, threadID.c_str());
    strcat(fmt, "]");
}

void
SiSLog::appendLabelLevel(char* fmt, const char* level)
{
    strcat(fmt, "[");
    strcat(fmt, level);
    strcat(fmt, "]");
}

void
SiSLog::appendLabelTag(char* fmt, const char* tag)
{
    strcat(fmt, "[");
    strcat(fmt, tag);
    strcat(fmt, "]");
}

void
SiSLog::appendLabelFunction(char* fmt, const char* function)
{
    strcat(fmt, "[");
    strcat(fmt, function);
    strcat(fmt, "]");
}

void
SiSLog::appendLabelFile(char* fmt, const char* file)
{
    strcat(fmt, "[");
    strcat(fmt, file);
    strcat(fmt, "]");
}

void
SiSLog::appendLabelLine(char* fmt, const int line)
{
    char linestr[64];
    sprintf(linestr, "%d", line);

    strcat(fmt, "[");
    strcat( fmt, linestr );
    strcat(fmt, "]");
}

void
SiSLog::appendLabelOwner(char* fmt)
{
    strcat(fmt, "[");
    strcat( fmt, m_owner.c_str() );
    strcat(fmt, "]");
}


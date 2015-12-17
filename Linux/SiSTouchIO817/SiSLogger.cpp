#include "SiSLogger.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

int m_LogInterface = 0;

int (*m_vLOGIpointer)(const char* format, va_list args) = 0;
int (*m_vLOGEpointer)(const char* format, va_list args) = 0;

#ifdef SDCARD_FILE_LOG
FILE* file_log;
#define LOG_LOCATION "/sdcard/SiS_logcat.txt"
#endif

int setLogInterface(int logInterface)
{

    #ifdef SDCARD_FILE_LOG
    if( (logInterface & FILE_LOG_FLAG) == 0 && (m_LogInterface & FILE_LOG_FLAG) != 0)
    {
        if(file_log)
         {
            fclose(file_log);
            file_log = 0;
         }
    }
    else if( (logInterface & FILE_LOG_FLAG) != 0 && (m_LogInterface & FILE_LOG_FLAG) == 0)
    {
        if(!file_log)
        {

            file_log = fopen( LOG_LOCATION, "a" );   

            if ( !file_log )     
            {
                LOGE( "open sdcard file failed, apk has no permission or /sdcard not exist" );
            }
            else
            {
                time_t day = time(0);
    
                fprintf( file_log, "==========\n" );
                fprintf( file_log, "%s", ctime(&day) );
                fflush( file_log );
            }
        }
    }
    else if( (logInterface & FILE_LOG_FLAG) != 0 && (m_LogInterface & FILE_LOG_FLAG) != 0)
    {

        if(file_log)
         {
            fclose(file_log);
            file_log = 0;
         }


        if(!file_log)
        {

            file_log = fopen( LOG_LOCATION, "a" );   

            if ( !file_log )     
            {
                LOGE( "open sdcard file failed, apk has no permission or /sdcard not exist" );
            }
            else
            {
                time_t day = time(0);
    
                fprintf( file_log, "==========\n" );
                fprintf( file_log, "%s", ctime(&day) );
                fflush( file_log );
            }
        }
    }
    #endif




    if(logInterface == ANDROID_LOG_FLAG)
    {
#ifdef FOR_ANDROID_LOG
        m_LogInterface = ANDROID_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoLogcat;
        m_vLOGEpointer = &vLOGEtoLogcat;
#else
        m_LogInterface = NULL_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoNull;
        m_vLOGEpointer = &vLOGEtoNull;;
#endif
    }
    else if(logInterface == FILE_LOG_FLAG)
    {
#ifdef SDCARD_FILE_LOG
        m_LogInterface = FILE_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoFile;
        m_vLOGEpointer = &vLOGEtoFile;
#else
        m_LogInterface = NULL_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoNull;
        m_vLOGEpointer = &vLOGEtoNull;
#endif
    }
    else if(logInterface == ANDROID_AND_FILE_LOG_FLAG)
    {
#if defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
        m_LogInterface = ANDROID_AND_FILE_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoLogcatAndFile;
        m_vLOGEpointer = &vLOGEtoLogcatAndFile;
#elif !defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
        m_LogInterface = FILE_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoFile;
        m_vLOGEpointer = &vLOGEtoFile;
#elif defined(FOR_ANDROID_LOG) && !defined(SDCARD_FILE_LOG)
        m_LogInterface = ANDROID_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoLogcat;
        m_vLOGEpointer = &vLOGEtoLogcat;
#else
        m_LogInterface = NULL_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoNull;
        m_vLOGEpointer = &vLOGEtoNull;
#endif
    }
    else
    {
        m_LogInterface = NULL_LOG_FLAG;
        m_vLOGIpointer = &vLOGItoNull;
        m_vLOGEpointer = &vLOGEtoNull;
    }



    LOGI( "LogInterface set to %d", m_LogInterface );
    return m_LogInterface;

}

int getLogInterface()
{
    return m_LogInterface;
}

int LOGI(const char* format, ...)
{
    if(m_vLOGIpointer)
    {
        va_list vl;
        va_start(vl, format);    
        (*m_vLOGIpointer)(format, vl);    
        va_end(vl);
        return 1;
    }
    else
    {
        return 0;
    }
}

int LOGE(const char* format, ...)
{
    if(m_vLOGEpointer)
    {
        va_list vl;
        va_start(vl, format);    
        (*m_vLOGEpointer)(format, vl);    
        va_end(vl);
        return 1;
    }
    else
    {
        return 0;
    }
}

#ifdef LOG_TIME

int  getCurrentTimeString(char * buf)
{
    timeval tv;
    tm stamp;
    gettimeofday(&tv, NULL);
    localtime_r(&(tv.tv_sec), &stamp);
    return sprintf(buf, "%02d-%d %d:%d:%d - %ld", stamp.tm_mon, stamp.tm_mday, stamp.tm_hour, stamp.tm_min, stamp.tm_sec, tv.tv_usec / 1000);
}

#endif

int vLOGItoNull(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#endif

    ret = vprintf(format, args);
    printf("\n");   
    fflush(stdout);  
    return ret;
}

int vLOGItoLogcat(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout); 
#ifdef FOR_ANDROID_LOG   
    ret =  __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, args);    
#endif
    return ret;
}

int vLOGItoFile(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        fprintf(file_log, "%s ", str);
    }
#endif
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);  
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {                      
        vfprintf(file_log, format, args);                
        ret = fprintf(file_log, "\n");
        fflush(file_log);
    }
#endif
    return ret;
}

int vLOGItoLogcatAndFile(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        fprintf(file_log, "%s " , str);
    }
#endif
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);;
#ifdef FOR_ANDROID_LOG
    ret =  __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, args);
#endif
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        vfprintf(file_log, format, args);
        ret = fprintf(file_log, "\n");
        fflush(file_log);
    }
#endif
    return ret;
}

int vLOGEtoNull(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);
    return ret;
}

int vLOGEtoLogcat(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);
#ifdef FOR_ANDROID_LOG
    ret =  __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, args);
#endif
    return ret;
}

int vLOGEtoFile(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        fprintf(file_log, "%s ", str);
    }
#endif
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        vfprintf(file_log, format, args);
        ret = fprintf(file_log, "\n");
        fflush(file_log);
    }
#endif
    return ret;
}

int vLOGEtoLogcatAndFile(const char* format, va_list args)
{
    int ret = 0;

#ifdef LOG_TIME
    char str [256]= {0};
    getCurrentTimeString(str);
    printf("%s ", str);
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        fprintf(file_log, "%s ", str);
    }
#endif
#endif

    ret = vprintf(format, args);
    printf("\n");
    fflush(stdout);
#ifdef FOR_ANDROID_LOG
    ret =  __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, args);
#endif
#ifdef SDCARD_FILE_LOG
    if (file_log)
    {
        vfprintf(file_log, format, args);
        ret = fprintf(file_log, "\n");
        fflush(file_log);
    }
#endif
    return ret;
}

int LOGItoNull(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);        
    ret = vLOGItoNull(format, vl);    
    va_end(vl);
    return ret;
}

#ifdef FOR_ANDROID_LOG
int LOGItoLogcat(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);        
    ret = vLOGItoLogcat(format, vl); 
    va_end(vl);
    return ret;
}
#endif

#ifdef SDCARD_FILE_LOG
int LOGItoFile(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);        
    ret = vLOGItoFile(format, vl);
    va_end(vl);
    return ret;
}
#endif

#if defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
int LOGItoLogcatAndFile(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);
    ret = vLOGItoLogcatAndFile(format, vl);
    va_end(vl);
    return ret;
}
#endif

int LOGEtoNull(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);
    ret = vLOGEtoNull(format, vl);
    va_end(vl);
    return ret;
}

#ifdef FOR_ANDROID_LOG
int LOGEtoLogcat(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);
    ret = vLOGEtoLogcat(format, vl);
    va_end(vl);
    return ret;
}
#endif

#ifdef SDCARD_FILE_LOG
int LOGEtoFile(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);    
    ret = vLOGEtoFile(format, vl);
    va_end(vl);
    return ret;
}
#endif

#if defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
int LOGEtoLogcatAndFile(const char* format, ...)
{
    int ret = 0;

    va_list vl;
    va_start(vl, format);
    ret = vLOGEtoLogcatAndFile(format, vl);
    va_end(vl);
    return ret;
}
#endif

void initialLogger()
{
    #if defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
        setLogInterface(ANDROID_AND_FILE_LOG_FLAG);
    #elif !defined(FOR_ANDROID_LOG) && defined(SDCARD_FILE_LOG)
        setLogInterface(NULL_LOG_FLAG);
    #elif defined(FOR_ANDROID_LOG) && !defined(SDCARD_FILE_LOG)
        setLogInterface(ANDROID_LOG_FLAG);
    #else
        setLogInterface(NULL_LOG_FLAG);
    #endif
}

void releaseLogger()
{
    m_LogInterface = 0;
    m_vLOGIpointer = 0;
    m_vLOGEpointer = 0;

    #ifdef SDCARD_FILE_LOG
    if ( file_log )     
    {
        fclose(file_log);
        file_log = 0;
    }
    #endif
}

#ifndef SISLOGGER_H
#define SISLOGGER_H

#include <stdio.h>
#include <stdarg.h>

// FOR_ANDROID_LOG & SDCARD_FILE_LOG is define in Android.mk (LOCAL_CFLAGS)
//#define FOR_ANDROID_LOG
//#define SDCARD_FILE_LOG
#define LOG_TIME

#ifdef FOR_ANDROID_LOG
#define LOG_TAG "SiSTouchjni native.cpp"
#include <android/log.h>
#endif

const int NULL_LOG_FLAG = 0x0;
const int ANDROID_LOG_FLAG = 0x1;
const int FILE_LOG_FLAG = 0x2;
const int ANDROID_AND_FILE_LOG_FLAG = 0x3;

int setLogInterface(int logInterface);
int getLogInterface();

void initialLogger();
void releaseLogger();

int LOGI(const char* format, ...);
int LOGE(const char* format, ...);


#ifdef LOG_TIME
int  getCurrentTimeString(char * buf);
#endif

int vLOGItoNull(const char* format, va_list args);
int vLOGItoLogcat(const char* format, va_list args);
int vLOGItoFile(const char* format, va_list args);
int vLOGItoLogcatAndFile(const char* format, va_list args);

int vLOGEtoNull(const char* format, va_list args);
int vLOGEtoLogcat(const char* format, va_list args);
int vLOGEtoFile(const char* format, va_list args);
int vLOGEtoLogcatAndFile(const char* format, va_list args);

int LOGItoNull(const char* format, ...);
int LOGItoLogcat(const char* format, ...);
int LOGItoFile(const char* format, ...);
int LOGItoLogcatAndFile(const char* format, ...);

int LOGEtoNull(const char* format, ...);
int LOGEtoLogcat(const char* format, ...);
int LOGEtoFile(const char* format, ...);
int LOGEtoLogcatAndFile(const char* format, ...);

#endif

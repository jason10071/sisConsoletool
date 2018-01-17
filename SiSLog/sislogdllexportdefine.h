#ifndef SISLOGDLLEXPORTDEFINE
#define SISLOGDLLEXPORTDEFINE

#if defined(WIN32) | defined(_WIN32)
    #ifdef DYNAMIC_SISLOG_LIBRARY
        #define SISLOG_EXPORT //__declspec(dllexport)
    #else
        #ifdef STATIC_SISLOG_LIBRARY
            #define SISLOG_EXPORT
        #else
            #define SISLOG_EXPORT //__declspec(dllimport)
        #endif
    #endif
#else
    #define SISLOG_EXPORT
#endif

#endif // SISLOGDLLEXPORTDEFINE


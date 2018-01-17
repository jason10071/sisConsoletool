#ifndef CTBASEDLLEXPORTDEFINE
#define CTBASEDLLEXPORTDEFINE

#if defined(WIN32) | defined(_WIN32)
    #ifdef DYNAMIC_CTBASE_LIBRARY
        #define CTBASE_EXPORT //__declspec(dllexport)
    #else
        #ifdef STATIC_CTBASE_LIBRARY
            #define CTBASE_EXPORT
        #else
            #define CTBASE_EXPORT //__declspec(dllimport)
        #endif
    #endif
#else
    #define CTBASE_EXPORT
#endif

#endif // CTBASEDLLEXPORTDEFINE


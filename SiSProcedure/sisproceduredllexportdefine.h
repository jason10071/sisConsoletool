#ifndef SISPROCEDUREDLLEXPORTDEFINE
#define SISPROCEDUREDLLEXPORTDEFINE

#if defined(WIN32) | defined(_WIN32)
    #ifdef DYNAMIC_SISPROCEDURE_LIBRARY
        #define SISPROCEDURE_EXPORT //__declspec(dllexport)
    #else
        #ifdef STATIC_SISPROCEDURE_LIBRARY
            #define SISPROCEDURE_EXPORT
        #else
            #define SISPROCEDURE_EXPORT //__declspec(dllimport)
        #endif
    #endif
#else
    #define SISPROCEDURE_EXPORT
#endif

#endif // SISPROCEDUREDLLEXPORTDEFINE


#ifndef SISCOREEXCEPTION
#define SISCOREEXCEPTION

#define SISCORE_EXCEPTION_SP_HANDLE \
    catch(SiSDeviceException & e )\
    {\
        std::string msg = EXCEPTION_TITLE;\
        char errorMsg[1024] = "";\
        sprintf(errorMsg, "SiSDeviceException: %s (ErrCode: %d)", e.what(), e.getErrCode());\
        msg.append(errorMsg);\
        throw SPException( msg );\
    }\
    catch(...)\
    {\
        std::string msg = EXCEPTION_TITLE;\
        char errorMsg[1024] = "";\
        sprintf(errorMsg, "SiSDeviceException unknow exception (...)");\
        msg.append(errorMsg);\
        throw SPException( msg );\
    }

#endif // SISCOREEXCEPTION


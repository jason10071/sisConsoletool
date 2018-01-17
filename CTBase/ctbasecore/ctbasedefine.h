#ifndef CTBASEDEFINE
#define CTBASEDEFINE

/* =============================================================================
* exitCode
============================================================================= */
#define USING_EXIT_CODE CTExitCode exitCode = CT_EXIT_AP_FLOW_ERROR;

#define EXIT_CODE exitCode

#define SHOW_RESULT \
switch (EXIT_CODE)\
{\
case CT_EXIT_PASS:\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_RESULT", "result : PASS");\
    break;\
case CT_EXIT_FAIL:\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_RESULT", "result : FAIL");\
    break;\
case CT_EXIT_CHIP_COMMUNICATION_ERROR:\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_RESULT", "$result : CHIP_COMMUNICATION_ERROR");\
    break;\
case CT_EXIT_AP_FLOW_ERROR:\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_RESULT", "$result : AP_FLOW_ERROR");\
    break;\
default:\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_RESULT", "$result : UNDO");\
    break;\
}\

#define RETURN_EXIT_CODE \
    SIS_LOG_I(SiSLog::getOwnerSiS(), "RETURN_EXIT_CODE", "");\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "RETURN_EXIT_CODE", "RETURN_EXIT_CODE: %d", EXIT_CODE);\
    SHOW_RESULT\
    SiSLog::getOwnerSiS()->releaseLogFile();\
    return EXIT_CODE;\

/* =============================================================================
* PARSE_ARGS
============================================================================= */
#define PARSE_ARGS(ctParameter, argc, argv)\
ctParameter->parseArgs(argc - 1, argv + 1);\
\
if( !ctParameter->getPraseErrorInformation().empty() )\
{\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "PARSE_ARGS", "PraseErrorInformation: %s", ctParameter->getPraseErrorInformation().c_str());\
}\

/* =============================================================================
* DEBUG_SETTING_IF_HAS_ARG_DEBUG
============================================================================= */
#define DEBUG_SETTING_IF_HAS_ARG_DEBUG_LOG(ctParameter)\
if( ctParameter->contains(ARG_DEBUG) && ctParameter->find(ARG_DEBUG)->getValueToBool() )\
{\
    SiSLog::getOwnerSiS()->setDebugVisible(true);\
    SiSLog::getOwnerSiS()->setLogFormat(SiSLog::FORMATE_DETAIL);\
}\
if( ctParameter->contains(ARG_LOG_PATH) && !ctParameter->find(ARG_LOG_PATH)->getValue().empty() )\
{\
    SiSLog::getOwnerSiS()->setLogType( LOG_TYPE_LOGCAT | LOG_TYPE_FILE );\
    SiSLog::getOwnerSiS()->setLogFile( ctParameter->find(ARG_LOG_PATH)->getValue().c_str() );\
}\

/* =============================================================================
* SHOW_HELP_IF_HAS_ARG_HELP
============================================================================= */
#define SHOW_HELP_IF_HAS_ARG_HELP(ctParameter)\
if( ctParameter->contains(ARG_HELP) && ctParameter->find(ARG_HELP)->getValueToBool() )\
{\
    ctParameter->showHelp();\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    RETURN_EXIT_CODE\
}\

/* =============================================================================
* PARSE_ARGUMENT
============================================================================= */
#define PARSE_ARGUMENT(ctParameter)\
try\
{\
    ctParameter->parseArgument();\
}\
catch(CTException ct)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "PARSE_ARGUMENT", "%s", ct.what());\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "PARSE_ARGUMENT", "\n--- help ---\n");\
    ctParameter->showHelp();\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    RETURN_EXIT_CODE\
}\
catch(SPException sp)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "PARSE_ARGUMENT", "%s", sp.what());\
    SIS_LOG_I(SiSLog::getOwnerSiS(), "PARSE_ARGUMENT", "\n--- help ---\n");\
    ctParameter->showHelp();\
    \
    if( sp.spcode() == SPException::SP_SISCORE_EXCEPTION )\
    { EXIT_CODE = CT_EXIT_CHIP_COMMUNICATION_ERROR; }\
    else { EXIT_CODE = CT_EXIT_AP_FLOW_ERROR; }\
    RETURN_EXIT_CODE\
}\
catch(...)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "PARSE_ARGUMENT", "...");\
    ctParameter->showHelp();\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    RETURN_EXIT_CODE\
}\

/* =============================================================================
* SHOW_CTCORE_VERSION
============================================================================= */
#define SHOW_CTCORE_VERSION \
SIS_LOG_I(SiSLog::getOwnerSiS(), "SHOW_CTCORE_VERSION", "# APP_VERSION : %s\n", APP_VERSION);\

/* =============================================================================
* ENABLE_CTL_REPORT_TO_OS
============================================================================= */
#define ENABLE_CTL_REPORT_TO_OS(ctCore)\
SIS_LOG_I(SiSLog::getOwnerSiS(), "ENABLE_CTL_REPORT_TO_OS", "ENABLE_CTL_REPORT_TO_OS :");\
try\
{\
    ctCore.destroy();\
}\
catch(CTException ct)\
{\
    SIS_LOG_D(SiSLog::getOwnerSiS(), "ENABLE_CTL_REPORT_TO_OS", "%s", ct.what());\
}\
catch(SPException sp)\
{\
    SIS_LOG_D(SiSLog::getOwnerSiS(), "ENABLE_CTL_REPORT_TO_OS", "%s", sp.what());\
}\
catch(...)\
{\
    SIS_LOG_D(SiSLog::getOwnerSiS(), "ENABLE_CTL_REPORT_TO_OS", "...");\
}\

/* =============================================================================
* CTCORE_INIT
============================================================================= */
#define CTCORE_INIT(ctCore)\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_INIT", "");\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_INIT", "CTCORE INIT :");\
try\
{\
    ctCore.init();\
}\
catch(CTException ct)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_INIT", "%s", ct.what());\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\
catch(SPException sp)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_INIT", "%s", sp.what());\
    \
    if( sp.spcode() == SPException::SP_SISCORE_EXCEPTION )\
    { EXIT_CODE = CT_EXIT_CHIP_COMMUNICATION_ERROR; }\
    else { EXIT_CODE = CT_EXIT_AP_FLOW_ERROR; }\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\
catch(...)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_INIT", "...");\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\

/* =============================================================================
* CTCORE_EXEC
============================================================================= */
#define CTCORE_EXEC(ctCore)\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_EXEC", "");\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_EXEC", "CTCORE EXEC :");\
try\
{\
    EXIT_CODE = ctCore.exec();\
}\
catch(CTException ct)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_EXEC", "%s", ct.what());\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\
catch(SPException sp)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_EXEC", "%s", sp.what());\
    \
    if( sp.spcode() == SPException::SP_SISCORE_EXCEPTION )\
    { EXIT_CODE = CT_EXIT_CHIP_COMMUNICATION_ERROR; }\
    else { EXIT_CODE = CT_EXIT_AP_FLOW_ERROR; }\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\
catch(...)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_EXEC", "...");\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    ENABLE_CTL_REPORT_TO_OS(ctCore)\
    RETURN_EXIT_CODE\
}\

/* =============================================================================
* CTCORE_DESTROY
============================================================================= */
#define CTCORE_DESTROY(ctCore)\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_DESTROY", "");\
SIS_LOG_I(SiSLog::getOwnerSiS(), "CTCORE_DESTROY", "CTCORE DESTROY :");\
try\
{\
    ctCore.destroy();\
}\
catch(CTException ct)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_DESTROY", "%s", ct.what());\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    RETURN_EXIT_CODE\
}\
catch(SPException sp)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_DESTROY", "%s", sp.what());\
    \
    if( sp.spcode() == SPException::SP_SISCORE_EXCEPTION )\
    { EXIT_CODE = CT_EXIT_CHIP_COMMUNICATION_ERROR; }\
    else { EXIT_CODE = CT_EXIT_AP_FLOW_ERROR; }\
    RETURN_EXIT_CODE\
}\
catch(...)\
{\
    SIS_LOG_E(SiSLog::getOwnerSiS(), "CTCORE_DESTROY", "...");\
    \
    EXIT_CODE = CT_EXIT_AP_FLOW_ERROR;\
    RETURN_EXIT_CODE\
}\

// =============================================================================

#endif // CTBASEDEFINE


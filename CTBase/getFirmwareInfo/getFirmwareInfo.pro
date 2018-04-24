QT       += sql

QT       -= gui

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

# == rename executable ==
TARGET = nGetFirmwareInfo
# =======================

SOURCES += \
    getFirmwareInfo.cpp \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_autotool.cpp \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_general.cpp \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_generalsingle.cpp \
    getfirmwareinfocore/getfirmwareinfocore.cpp \
    getfirmwareinfocore/getfirmwareinfoparameter.cpp \
    getfirmwareinfocore/getfirmwareinforeference.cpp \
    getfirmwareinfocore/igetfirmwareinfocore.cpp

HEADERS += \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_autotool.h \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_general.h \
    getfirmwareinfocore/flowversion/getfirmwareinfocore_generalsingle.h \
    getfirmwareinfocore/getfirmwareinfocore.h \
    getfirmwareinfocore/getfirmwareinfoparameter.h \
    getfirmwareinfocore/getfirmwareinforeference.h \
    getfirmwareinfocore/igetfirmwareinfocore.h

#============================================================================================
# include apinfo.pri
include(../apinfo.pri)
#============================================================================================
include(deployment.pri)
qtcAddDeployment()
#============================================================================================
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
#============================================================================================
# build lib path
#============================================================================================
isEmpty(Lib_path) {
        Lib_path = $$PWD/../../../../Lib
}

include($$Lib_path/include/qmake/utility/SiSUtility.pri)
#============================================================================================
#============================================================================================
# include apCommonLib.pri
include(../apCommonLib.pri)
#============================================================================================
#============================================================================================
# include AndroidConsoleSettings.pri
android {
    include($$Lib_path/include/qmake/utility/AndroidConsoleSettings.pri)
}
#============================================================================================

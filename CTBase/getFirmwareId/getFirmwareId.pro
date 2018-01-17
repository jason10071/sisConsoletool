QT       += sql

QT       -= gui

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

# == rename executable ==
TARGET = nGetFirmwareId
# =======================

SOURCES += \
    getFirmwareId.cpp \
    getfirmwareidcore/flowversion/getfirmwareidcore_general.cpp \
    getfirmwareidcore/flowversion/getfirmwareidcore_generalsingle.cpp \
    getfirmwareidcore/getfirmwareidcore.cpp \
    getfirmwareidcore/getfirmwareidparameter.cpp \
    getfirmwareidcore/getfirmwareidreference.cpp \
    getfirmwareidcore/igetfirmwareidcore.cpp

HEADERS += \
    getfirmwareidcore/flowversion/getfirmwareidcore_general.h \
    getfirmwareidcore/flowversion/getfirmwareidcore_generalsingle.h \
    getfirmwareidcore/getfirmwareidcore.h \
    getfirmwareidcore/getfirmwareidparameter.h \
    getfirmwareidcore/getfirmwareidreference.h \
    getfirmwareidcore/igetfirmwareidcore.h

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

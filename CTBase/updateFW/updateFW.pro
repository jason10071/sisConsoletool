QT       += sql

QT       -= gui

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

# == rename executable ==
TARGET = nUpdateFW
# =======================

SOURCES += \
    updateFW.cpp \
    updatefwcore/flowversion/updatefwcore_819.cpp \
    updatefwcore/flowversion/updatefwcore_819single.cpp \
    updatefwcore/flowversion/updatefwcore_general.cpp \
    updatefwcore/flowversion/updatefwcore_generalsingle.cpp \
    updatefwcore/iupdatefwcore.cpp \
    updatefwcore/updatefwcore.cpp \
    updatefwcore/updatefwflag.cpp \
    updatefwcore/updatefwparameter.cpp \
    updatefwcore/updatefwreference.cpp

HEADERS += \
    updatefwcore/flowversion/updatefwcore_819.h \
    updatefwcore/flowversion/updatefwcore_819single.h \
    updatefwcore/flowversion/updatefwcore_general.h \
    updatefwcore/flowversion/updatefwcore_generalsingle.h \
    updatefwcore/iupdatefwcore.h \
    updatefwcore/updatefwcore.h \
    updatefwcore/updatefwflag.h \
    updatefwcore/updatefwparameter.h \
    updatefwcore/updatefwreference.h

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

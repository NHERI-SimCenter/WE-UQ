#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network

TARGET = WE_UQ
TEMPLATE = app

CONFIG += debug

macos:LIBS += /usr/lib/libcurl.dylib
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

INCLUDEPATH += StochasticWindModel/include

win32 {
    RC_ICONS = icons/NHERI-CWE-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-CWE-Icon.icns
    }
}

include(../EE-UQ/CommonWorkflow.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(./MiniZip/MiniZip.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    WorkflowAppWE.cpp \
    RunWidget.cpp \
    WindEventSelection.cpp \
    EDP_WindSelection.cpp \
    StandardWindEDP.cpp \
    DEDM_HRP.cpp \
    StochasticWindModel/src/KwonKareem2006.cpp \
    StochasticWindModel/src/StochasticWindInput.cpp


HEADERS  += \
    WorkflowAppWE.h \
    RunWidget.h \
    WindEventSelection.h \
    StandardWindEDP.h \
    EDP_WindSelection.h \
    DEDM_HRP.h \
    StochasticWindModel/include/KwonKareem2006.h \
    StochasticWindModel/include/StochasticWindInput.h


RESOURCES += \
    ../EE-UQ/images.qrc \
    images.qrc \

#FORMS    += mainwindow.ui

#RESOURCES += \
#    schema.qrc

DISTFILES += \
    wImage2.gif \
    wImage3.gif \
    wImage4.gif \
    wImage1.png \
    wImage_DEDM_HRP_Logo




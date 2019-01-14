#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network

TARGET = CWE
TEMPLATE = app

macos:LIBS += /usr/lib/libcurl.dylib
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

win32 {
    RC_ICONS = icons/NHERI-CWE-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-CWE-Icon.icns
    }
}

include(../EE-UQ/EE-UQ.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(../GroundMotionUtilities/UI/GroundMotionWidgets.pri)
include(./MiniZip/MiniZip.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    WorkflowAppCWE.cpp \
    RunWidget.cpp \
    WindEventSelection.cpp \
    DEDM_HRP.cpp


HEADERS  += \
    WorkflowAppCWE.h \
    RunWidget.h \
    WindEventSelection.h \
    DEDM_HRP.h


RESOURCES += \
    ../EE-UQ/images.qrc \

#FORMS    += mainwindow.ui

#RESOURCES += \
#    schema.qrc

DISTFILES += \
    wImage2.gif \
    wImage3.gif \
    wImage4.gif \
    wImage1.png \
    wImage_DEDM_HRP_Logo




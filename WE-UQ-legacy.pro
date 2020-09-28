#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network 3dcore 3drender 3dextras printsupport

#CONFIG += debug_and_release
CONFIG += c++11

TARGET = WE-UQ
TEMPLATE = app

macos:LIBS += /usr/lib/libcurl.dylib
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
#win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

win32:INCLUDEPATH += "../curl-7.59/include"
win32:LIBS += "../curl-7.59/lib/libcurl_a.lib"
win32:DEFINES +=  CURL_STATICLIB

win32:INCLUDEPATH += "../jansson/include"
win32:LIBS += "../jansson/lib/jansson.lib"

INCLUDEPATH += StochasticWindModel/include
INCLUDEPATH += Inflow
INCLUDEPATH += CFD
INCLUDEPATH += CFD/UI
INCLUDEPATH += CFD/Analysis
INCLUDEPATH += CFD/CFDanalysis
INCLUDEPATH += CFD/SimCenter_widgets
DEFINES += _GRAPHICS_Qt3D

win32 {
    RC_ICONS = icons/NHERI-WEuq-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-WEuq-Icon.icns
    }
}

include(../SimCenterCommon/Workflow/Workflow.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(./WindEvents.pri)
# include(./MiniZip/MiniZip.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    EVENTS/usermodeshapes.cpp \
    WorkflowAppWE.cpp \
    RunWidget.cpp \
    WindEDP_Selection.cpp \
    StandardWindEDP.cpp

HEADERS  += \
    EVENTS/usermodeshapes.h \
    WorkflowAppWE.h \
    RunWidget.h \
    StandardWindEDP.h \
    WindEDP_Selection.h

RESOURCES += \
    ../EE-UQ/styles.qrc \
    images.qrc

#    ../EE-UQ/images.qrc \
#    we-uq-resources.qrc

#FORMS    += mainwindow.ui


#RESOURCES += \
#    schema.qrc

DISTFILES += \
    WEUQinstaller/InstallScripts/README.txt \
    WEUQinstaller/InstallScripts/makeInstallPackageWEUQ-with-QWT.sh \
    WEUQinstaller/InstallScripts/makeInstallPackageWEUQ.sh \
    WEUQinstaller/config/NHERI-WEuq-Icon.icns \
    WEUQinstaller/config/NHERI-WEuq-Icon.png \
    WEUQinstaller/config/WEUQInstallerLogo.png \
    WEUQinstaller/config/macconfig.xml \
    WEUQinstaller/config/nheri_weuq_icon.ico \
    WEUQinstaller/config/watermark.png \
    WEUQinstaller/config/winconfig.xml \
    WEUQinstaller/packages/nheri.simcenter.python/data/python.fake \
    WEUQinstaller/packages/nheri.simcenter.python/meta/installPython.qs \
    WEUQinstaller/packages/nheri.simcenter.python/meta/license.txt \
    WEUQinstaller/packages/nheri.simcenter.python/meta/package.xml \
    WEUQinstaller/packages/nheri.simcenter.weuq/meta/installWEUQ.qs \
    WEUQinstaller/packages/nheri.simcenter.weuq/meta/license.txt \
    WEUQinstaller/packages/nheri.simcenter.weuq/meta/package.xml \
    wImage2.gif \
    wImage3.gif \
    wImage4.gif \
    wImage1.png \
    wImage_DEDM_HRP_Logo

FORMS += \
    EVENTS/usermodeshapes.ui


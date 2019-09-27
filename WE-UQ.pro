#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network 3dcore 3drender 3dextras

CONFIG += debug_and_release
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
INCLUDEPATH += CFD/CFDAnalysis
INCLUDEPATH += CFD/SimCenter_widgets

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
include(./MiniZip/MiniZip.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    CFD/CFDanalysis/CFDanalysisType.cpp \
    CFD/CFDanalysis/CFDcaseInstance.cpp \
    CFD/UI/CFDExpertWidget.cpp \
    CFD/UI/CWE3DView.cpp \
    CFD/UI/Edged3DBox.cpp \
    CFD/UI/GeometryHelper.cpp \
    CFD/UI/CFDTemplateWidget.cpp \
    CFD/UI/Graphics3DAxes.cpp \
    CFD/UI/PatchesSelector.cpp \
    CFD/UI/RemoteCaseSelector.cpp \
    CFD/UI/SubdomainsModel.cpp \
    CFD/cfd_globals.cpp \
    CFD/cwe_guiWidgets/cwe_parameters.cpp \
    CFD/cwe_guiWidgets/cwe_state_label.cpp \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.cpp \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.cpp \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.cpp \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.cpp \
    Inflow/inflowparameterwidget.cpp \
    WorkflowAppWE.cpp \
    RunWidget.cpp \
    WindEventSelection.cpp \
    WindEDP_Selection.cpp \
    StandardWindEDP.cpp \
    DEDM_HRP.cpp \
    CWE.cpp \
    MeshParametersCWE.cpp \
    SimulationParametersCWE.cpp \
    LowRiseTPU.cpp \
    WindTunnelExperiment.cpp \
    StochasticWindModel/src/WittigSinha.cpp \
    StochasticWindModel/src/StochasticWindInput.cpp \
    CFD/Analysis/filemetadata.cpp \
    CFD/SimCenter_widgets/sctrbooldatawidget.cpp \
    CFD/SimCenter_widgets/sctrchoicedatawidget.cpp \
    CFD/SimCenter_widgets/sctrfiledatawidget.cpp \
    CFD/SimCenter_widgets/sctrmasterdatawidget.cpp \
    CFD/SimCenter_widgets/sctrstddatawidget.cpp \
    CFD/SimCenter_widgets/sctrtextdatawidget.cpp \
    CFD/Analysis/remotejobdata.cpp


HEADERS  += \
    CFD/CFDanalysis/CFDanalysisType.h \
    CFD/CFDanalysis/CFDcaseInstance.h \
    CFD/SimCenter_widgets/sctrstates.h \
    CFD/UI/CFDTemplateWidget.h \
    CFD/UI/CFDExpertWidget.h \
    CFD/UI/CWE3DView.h \
    CFD/UI/Edged3DBox.h \
    CFD/UI/GeometryHelper.h \
    CFD/UI/Graphics3DAxes.h \
    CFD/UI/PatchesSelector.h \
    CFD/UI/RemoteCaseSelector.h \
    CFD/UI/SubdomainsModel.h \
    CFD/cfd_globals.h \
    CFD/cwe_guiWidgets/cwe_parameters.h \
    CFD/cwe_guiWidgets/cwe_state_label.h \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.h \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.h \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.h \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.h \
    Inflow/inflowparameterwidget.h \
    WorkflowAppWE.h \
    RunWidget.h \
    WindEventSelection.h \
    WindTunnelExperiment.h \
    StandardWindEDP.h \
    WindEDP_Selection.h \
    DEDM_HRP.h \
    CWE.h \
    MeshParametersCWE.h \
    SimulationParametersCWE.h \
    LowRiseTPU.h \
    StochasticWindModel/include/WittigSinha.h \
    StochasticWindModel/include/StochasticWindInput.h \
    CFD/Analysis/filemetadata.h \
    CFD/SimCenter_widgets/sctrbooldatawidget.h \
    CFD/SimCenter_widgets/sctrchoicedatawidget.h \
    CFD/SimCenter_widgets/sctrfiledatawidget.h \
    CFD/SimCenter_widgets/sctrmasterdatawidget.h \
    CFD/SimCenter_widgets/sctrstddatawidget.h \
    CFD/SimCenter_widgets/sctrtextdatawidget.h \
    CFD/Analysis/remotejobdata.h


RESOURCES += \
    ../EE-UQ/styles.qrc \
    images.qrc

#    ../EE-UQ/images.qrc \
#    we-uq-resources.qrc

#FORMS    += mainwindow.ui
FORMS    += \
    CFD/cwe_guiWidgets/cwe_parameters.ui \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.ui \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.ui \
    CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.ui \
    Inflow/inflowparameterwidget.ui

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


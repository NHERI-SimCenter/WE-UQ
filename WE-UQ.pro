#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network

TARGET = WE_UQ
TEMPLATE = app

macos:LIBS += /usr/lib/libcurl.dylib
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

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

include(../EE-UQ/CommonWorkflow.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(./MiniZip/MiniZip.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    CFD/CFDanalysis/CFDanalysisType.cpp \
    CFD/CFDanalysis/CFDcaseInstance.cpp \
    CFD/UI/CFDExpertWidget.cpp \
    CFD/UI/CFDTemplateWidget.cpp \
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
    EDP_WindSelection.cpp \
    StandardWindEDP.cpp \
    DEDM_HRP.cpp \
    StochasticWindModel/src/WittigSinha.cpp \
    StochasticWindModel/src/StochasticWindInput.cpp \
    CFD/cwe_interfacedriver.cpp \
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
    StandardWindEDP.h \
    EDP_WindSelection.h \
    DEDM_HRP.h \
    StochasticWindModel/include/WittigSinha.h \
    StochasticWindModel/include/StochasticWindInput.h \
    CFD/cwe_interfacedriver.h \
    CFD/Analysis/filemetadata.h \
    CFD/SimCenter_widgets/sctrbooldatawidget.h \
    CFD/SimCenter_widgets/sctrchoicedatawidget.h \
    CFD/SimCenter_widgets/sctrfiledatawidget.h \
    CFD/SimCenter_widgets/sctrmasterdatawidget.h \
    CFD/SimCenter_widgets/sctrstddatawidget.h \
    CFD/SimCenter_widgets/sctrtextdatawidget.h \
    CFD/Analysis/remotejobdata.h


RESOURCES += \
    ../EE-UQ/images.qrc \
    images.qrc \
    we-uq-resources.qrc

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
    wImage2.gif \
    wImage3.gif \
    wImage4.gif \
    wImage1.png \
    wImage_DEDM_HRP_Logo


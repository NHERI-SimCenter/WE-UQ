#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

CONFIG += conan_basic_setup
include(conanbuildinfo.pri)

QT       += core gui charts concurrent network

TARGET = WE_UQ
TEMPLATE = app


INCLUDEPATH += StochasticWindModel/include
INCLUDEPATH += Inflow


win32 {
    LIBS +=  -lAdvapi32
    RC_ICONS = icons/NHERI-WEuq-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-WEuq-Icon.icns
    }
}


INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    CFD/UI/CFDExpertWidget.cpp \
    CFD/UI/RemoteCaseSelector.cpp \
    Inflow/inflowparameterwidget.cpp \
    WorkflowAppWE.cpp \
    RunWidget.cpp \
    WindEventSelection.cpp \
    EDP_WindSelection.cpp \
    StandardWindEDP.cpp \
    DEDM_HRP.cpp \
    StochasticWindModel/src/WittigSinha.cpp \
    StochasticWindModel/src/StochasticWindInput.cpp


HEADERS  += \
    CFD/UI/CFDExpertWidget.h \
    CFD/UI/RemoteCaseSelector.h \
    Inflow/inflowparameterwidget.h \
    WorkflowAppWE.h \
    RunWidget.h \
    WindEventSelection.h \
    StandardWindEDP.h \
    EDP_WindSelection.h \
    DEDM_HRP.h \
    StochasticWindModel/include/WittigSinha.h \
    StochasticWindModel/include/StochasticWindInput.h


RESOURCES += \
    images.qrc \
    we-uq-resources.qrc

#FORMS    += mainwindow.ui

#RESOURCES += \
#    schema.qrc

DISTFILES += \
    wImage2.gif \
    wImage3.gif \
    wImage4.gif \
    wImage1.png \
    wImage_DEDM_HRP_Logo

FORMS += \
    Inflow/inflowparameterwidget.ui


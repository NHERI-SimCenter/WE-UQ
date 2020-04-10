QT += testlib
QT += core gui charts concurrent network 3dcore 3drender 3dextras printsupport
CONFIG += qt warn_on depend_includepath testcase file_copies

include($$PWD/ConanHelper.pri)

#Disable m64 flag on windows
win32:QMAKE_CXXFLAGS -= -m64 #TODO: This seems to be cause by smelt Conan package

TEMPLATE = app


INCLUDEPATH += StochasticWindModel/include
INCLUDEPATH += Inflow
INCLUDEPATH += CFD
INCLUDEPATH += CFD/UI
INCLUDEPATH += CFD/Analysis
INCLUDEPATH += CFD/CFDanalysis
INCLUDEPATH += CFD/SimCenter_widgets

win32 {
    LIBS +=  -lAdvapi32
    RC_ICONS = icons/NHERI-WEuq-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-WEuq-Icon.icns
    }
}

include(./WindEvents.pri)

SOURCES += WorkflowAppWE.cpp \
    RunWidget.cpp \
    WindEDP_Selection.cpp \
    StandardWindEDP.cpp

HEADERS  += \
    WorkflowAppWE.h \
    RunWidget.h \
    StandardWindEDP.h \
    WindEDP_Selection.h


SOURCES +=  $$PWD/tests/workflowtests.cpp

examples.files = example/*
examples.path = $$OUT_PWD/example
COPIES += examples

OTHER_FILES += $$PWD/example/stochastic-example.json

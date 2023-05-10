#-------------------------------------------------
#
# Project created by abiy
#
#-------------------------------------------------

#message($$_PRO_FILE_PWD_)
#message($$PWD)

INCLUDEPATH += $$PWD/EVENTS
INCLUDEPATH += $$PWD/EVENTS/IsolatedBuildingCFD
INCLUDEPATH += $$PWD/WindResults


SOURCES += \
    $$PWD/WindResults/WindResults.cpp \
    $$PWD/WindResults/IsolatedBuildingCFDResults.cpp \

HEADERS  += \
    $$PWD/WindResults/WindResults.h \
    $$PWD/WindResults/IsolatedBuildingCFDResults.h \


#VTK Includes and libraries
INCLUDEPATH += /usr/local/include/vtk-9.2

LIBS += -L/usr/local/lib \
        -lvtkViewsCore-9.2 \
        -lvtkCommonCore-9.2 \
        -lvtkFiltersGeometry-9.2 \
        -lvtkRenderingCore-9.2 \
        -lvtkRenderingContextOpenGL2-9.2 \
        -lvtkCommonExecutionModel-9.2 \
        -lvtkCommonDataModel-9.2 \
        -lvtkGUISupportQt-9.2 \
        -lvtkRenderingOpenGL2-9.2 \
        -lvtkIOGeometry-9.2 \
        -lvtkViewsQt-9.2 \
        -lvtkIOLegacy-9.2 \
        -lvtksys-9.2

QT += widgets
QT += core gui opengl



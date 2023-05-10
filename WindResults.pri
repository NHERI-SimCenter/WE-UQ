#-------------------------------------------------
#
# Project created by abiy
#
#-------------------------------------------------

#message($$_PRO_FILE_PWD_)
#message($$PWD)

INCLUDEPATH += $$PWD/EVENTS
INCLUDEPATH += $$PWD/EVENTS/IsolatedBuildingCFD
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD
INCLUDEPATH += $$PWD/WindResults


SOURCES += \
    $$PWD/WindResults/WindResults.cpp \
    $$PWD/WindResults/IsolatedBuildingCFDResults.cpp \

HEADERS  += \
    $$PWD/WindResults/WindResults.h \
    $$PWD/WindResults/IsolatedBuildingCFDResults.h \





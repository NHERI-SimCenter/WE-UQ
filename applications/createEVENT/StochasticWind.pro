TEMPLATE = app
CONFIG += console

DESTDIR  = $$PWD

INCLUDEPATH += $$PWD/StochasticWindGenerator/include \
               $$PWD/StochasticWindGenerator/external/Clara \
               $$PWD/StochasticWindGenerator/external \
               $$PWD/../../../SimCenterBackendApplications/common
                     
SOURCES += $$PWD/StochasticWindGenerator/src/command_parser.cc \
           $$PWD/StochasticWindGenerator/src/wind_generator.cc \
           $$PWD/StochasticWindGenerator/src/floor_forces.cc \           
           $$PWD/StochasticWindGenerator/src/main.cc \
           $$PWD/../../../SimCenterBackendApplications/common/Units.cpp           

unix{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.so
}
unix:macx{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.dylib
}

TEMPLATE = app
CONFIG += console
CONFIG += debug_and_release

DESTDIR  = $$PWD

INCLUDEPATH += $$PWD/StochasticWindGenerator/include \
               $$PWD/StochasticWindGenerator/external/Clara \
               $$PWD/StochasticWindGenerator/external \
               $$PWD/../common
                     
SOURCES += $$PWD/StochasticWindGenerator/src/command_parser.cc \
           $$PWD/StochasticWindGenerator/src/wind_generator.cc \
           $$PWD/StochasticWindGenerator/src/floor_forces.cc \           
           $$PWD/StochasticWindGenerator/src/main.cc \
           $$PWD/../common/Units.cpp           

unix{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.so
}
unix:macx{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.dylib
}

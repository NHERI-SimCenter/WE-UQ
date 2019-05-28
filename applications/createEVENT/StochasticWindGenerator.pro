TEMPLATE = app
CONFIG += console

DESTDIR  = $$PWD

INCLUDEPATH += $$PWD/StochasticWindGenerator/include \
               $$PWD/StochasticWindGenerator/external/Clara \
               $$PWD/StochasticWindGenerator/external

SOURCES += $$PWD/StochasticWindGenerator/src/command_parser.cc \
           $$PWD/StochasticWindGenerator/src/eq_generator.cc \
           $$PWD/StochasticWindGenerator/src/main.cc

unix{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.so
}
unix:macx{
    INCLUDEPATH += $$PWD/StochasticWindGenerator/external/smelt/include
    LIBS += $$PWD/StochasticWindGenerator/external/smelt/lib/libsmelt.dylib
}

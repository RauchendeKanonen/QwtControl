#-------------------------------------------------
#
# Project created by QtCreator 2012-11-05T13:10:09
#
#-------------------------------------------------

QT       += core

TARGET = differerncetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
INCLUDEPATH += /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4
LIBS += -L/usr/lib/ \
    -L. \
    -lqwt \
    -lm \
    -lginac

HEADERS +=

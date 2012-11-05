#-------------------------------------------------
#
# Project created by QtCreator 2012-11-05T13:10:09
#
#-------------------------------------------------

QT       += core
QT       -= gui
TARGET = differerncetest
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
INCLUDEPATH += /usr/include/qt4
LIBS += -L/usr/lib/ \
    -L. \
    -lm

HEADERS +=

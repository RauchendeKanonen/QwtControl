#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T18:27:22
#
#-------------------------------------------------

QT       += core
QT       -= gui
TARGET = dcs-cl
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../../numeric/numericallaplace.cpp \
    ../../mathFunction/mathfunctionevaluator.cpp \
    ../../mathFunction/mathfunctioncompiler.cpp \
    ../../util/tools.cpp

HEADERS += \
    ../../numeric/numericallaplace.h \
    ../../mathFunction/mathfunctionevaluator.h \
    ../../mathFunction/mathfunctioncompiler.h \
    ../../util/tools.h


INCLUDEPATH += ../../muparserx_v2_1_6/parser \
    ../../util \
    ../../ \
    ../../numeric \
LIBS += -L/usr/lib/ \
    -L. \
    -L ../../muparserx_v2_1_6/bin \
    -lmuparserx \
    -lm

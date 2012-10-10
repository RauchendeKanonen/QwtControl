#-------------------------------------------------
#
# Project created by QtCreator 2012-10-02T14:42:15
#
#-------------------------------------------------

QT       += core gui

TARGET = QwtControl
TEMPLATE = app
CONFIG += debug

SOURCES += main.cpp\
        mainwindow.cpp \
    vardialog.cpp \
    stepresponsedialog.cpp \
    bodedialog.cpp \
    expressiondialog.cpp \
    expressionmodel.cpp \
    varmodel.cpp \
    curvethread.cpp \
    rangeselectordialog.cpp \
    parametersliderdialog.cpp \
    qparameterslider.cpp \
    numericallaplace.cpp \
    curvesegmentthread.cpp \
    curvemodel.cpp

HEADERS  += mainwindow.h \
    vardialog.h \
    stepresponsedialog.h \
    bodedialog.h \
    expressiondialog.h \
    expressionmodel.h \
    varmodel.h \
    curvethread.h \
    rangeselectordialog.h \
    parametersliderdialog.h \
    qparameterslider.h \
    numericallaplace.h \
    curvesegmentthread.h \
    curvemodel.h \
    definitions.h

FORMS    += mainwindow.ui \
    vardialog.ui \
    bodedialog.ui \
    expressiondialog.ui \
    stepresponsedialog.ui \
    rangeselectordialog.ui \
    parametersliderdialog.ui

INCLUDEPATH += /usr/include/qwt5/ \
                ../../muparserx-read-only/parser

LIBS += -L/usr/lib/ \
    -L. \
    -lqwt \
    -lmuparser \
    -lmuparserx \
    -lm

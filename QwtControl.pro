# -------------------------------------------------
# Project created by QtCreator 2012-10-02T14:42:15
# -------------------------------------------------
QT += core \
    gui
TARGET = QwtControl
TEMPLATE = app
CONFIG += debug
SOURCES += main.cpp \
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
    curvemodel.cpp \
    mathFunction/mathfunctionpreprocessor.cpp \
    mathFunction/mathfunctionevaluator.cpp \
    mathFunction/mathfunctioncompiler.cpp \
    colorlisteditor.cpp \
    qwt_plot_root_locus_curve.cpp
HEADERS += mainwindow.h \
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
    curvemodel.h \
    definitions.h \
    mathFunction/mathfunctionpreprocessor.h \
    mathFunction/mathfunctionevaluator.h \
    mathFunction/mathfunctioncompiler.h \
    colorlisteditor.h \
    qwt_plot_root_locus_curve.h
FORMS += mainwindow.ui \
    vardialog.ui \
    bodedialog.ui \
    expressiondialog.ui \
    stepresponsedialog.ui \
    rangeselectordialog.ui \
    parametersliderdialog.ui
INCLUDEPATH += /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4 \
    muparserx-read-only/parser
LIBS += -L/usr/lib/ \
    -L. \
    -L./muparserx-read-only/bin \
    -lqwt-qt4 \
    -lmuparserx \
    -lm

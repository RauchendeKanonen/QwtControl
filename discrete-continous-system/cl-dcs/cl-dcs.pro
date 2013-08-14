#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T18:27:22
#
#-------------------------------------------------
QT += core \
    gui

TARGET = dcs-cl
CONFIG   += debug


TEMPLATE = app


SOURCES += main.cpp \
    ../../numeric/numericallaplace.cpp \
    ../../mathFunction/mathfunctionevaluator.cpp \
    ../../mathFunction/mathfunctioncompiler.cpp \
    ../../util/tools.cpp \
    ../../numeric/newton.cpp \
    ../../numeric/numericallaplace_weeks.cpp \
    ../../numeric/numericallaplace_gaverstehfest.cpp \
    ../../Dialogs/weeksetupdialog.cpp \
    ../../mathFunction/controlexpression.cpp \
    rtresponcewindow.cpp

HEADERS += \
    ../../numeric/numericallaplace.h \
    ../../mathFunction/mathfunctionevaluator.h \
    ../../mathFunction/mathfunctioncompiler.h \
    ../../util/tools.h \
    ../../numeric/newton.h \
    ../../Dialogs/weeksetupdialog.h \
    ../../mathFunction/controlexpression.h \
    rtresponcewindow.h


INCLUDEPATH += ../../muparserx_v2_1_6/parser \
    ../../util \
    ../../ \
    ../../numeric \
    ../../Dialogs \
    ../../muparserx_v2_1_6/parser \
    ../../util \
    /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4

LIBS += -L/usr/lib/ \
    -L. \
    -L ../../muparserx_v2_1_6/bin \
    -lmuparserx \
    -lm \
    -lginac \
    -lqwt


FORMS += \
    ../../Dialogs/weeksetupdialog.ui \
    rtresponcewindow.ui

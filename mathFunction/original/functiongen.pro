### eqmake4 was here ###
CONFIG += debug

TEMPLATE = app
CONFIG += console
CONFIG += qt

SOURCES += main.cpp \
    mathfunctioncompiler.cpp \
    mathfunctionevaluator.cpp \
    mathfunctionpreprocessor.cpp

HEADERS += \
    mathfunctioncompiler.h \
    mathfunctionevaluator.h \
    mathfunctionpreprocessor.h

INCLUDEPATH += /usr/include/qwt5/ \
                /usr/include/qwt-qt4/ \
                /usr/include/qt4 \
               ../svnqwtcon/qwtcontrol/trunk/muparserx-read-only/parser

LIBS += -L/usr/lib/ \
    -L. \
    -L../svnqwtcon/qwtcontrol/trunk/muparserx-read-only/bin \
    -lqwt \
    -lmuparserx \
    -lm

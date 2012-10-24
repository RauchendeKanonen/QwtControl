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
    rangeselectordialog.cpp \
    parametersliderdialog.cpp \
    qparameterslider.cpp \
    numericallaplace.cpp \
    curvemodel.cpp \
    mathFunction/mathfunctionevaluator.cpp \
    mathFunction/mathfunctioncompiler.cpp \
    colorlisteditor.cpp \
    controlexpression.cpp \
    csv.cpp \
    csvdialog.cpp \
    qwt_root_locus_curve.cpp \
    qwt_response_curve.cpp \
    qwt_control_plot_item.cpp \
    qwt_zeta_curve.cpp \
    qwt_dataset_curve.cpp \
    qwt_magnitude_curve.cpp \
    qwt_phase_curve.cpp
HEADERS += mainwindow.h \
    vardialog.h \
    stepresponsedialog.h \
    bodedialog.h \
    expressiondialog.h \
    expressionmodel.h \
    varmodel.h \
    rangeselectordialog.h \
    parametersliderdialog.h \
    qparameterslider.h \
    numericallaplace.h \
    curvemodel.h \
    definitions.h \
    mathFunction/mathfunctionevaluator.h \
    mathFunction/mathfunctioncompiler.h \
    colorlisteditor.h \
    controlexpression.h \
    csv.h \
    csvdialog.h \
    qwt_root_locus_curve.h \
    qwt_response_curve.h \
    qwt_control_plot_item.h \
    qwt_zeta_curve.h \
    qwt_dataset_curve.h \
    qwt_magnitude_curve.h \
    qwt_phase_curve.h
FORMS += mainwindow.ui \
    vardialog.ui \
    bodedialog.ui \
    expressiondialog.ui \
    stepresponsedialog.ui \
    rangeselectordialog.ui \
    parametersliderdialog.ui \
    csvdialog.ui
INCLUDEPATH += /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4
LIBS += -L/usr/lib/ \
    -L. \
    -lqwt \
    -lm \
    -lginac

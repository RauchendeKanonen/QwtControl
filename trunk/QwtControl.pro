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
    Dialogs/vardialog.cpp \
    Dialogs/stepresponsedialog.cpp \
    Dialogs/bodedialog.cpp \
    Dialogs/expressiondialog.cpp \
    expressionmodel.cpp \
    varmodel.cpp \
    Dialogs/rangeselectordialog.cpp \
    Dialogs/parametersliderdialog.cpp \
    numericallaplace.cpp \
    curvemodel.cpp \
    mathFunction/mathfunctionevaluator.cpp \
    mathFunction/mathfunctioncompiler.cpp \
    colorlisteditor.cpp \
    controlexpression.cpp \
    csv.cpp \
    Dialogs/csvdialog.cpp \
    Curves/qwt_root_locus_curve.cpp \
    Curves/qwt_response_curve.cpp \
    Curves/qwt_zeta_curve.cpp \
    Curves/qwt_dataset_curve.cpp \
    Curves/qwt_magnitude_curve.cpp \
    Curves/qwt_phase_curve.cpp \
    Curves/qwt_curve_fitter.cpp \
    Dialogs/expressionclonedialog.cpp \
    Curves/qwt_control_plot_item.cpp \
    Curves/qwt_discrete_response_curve.cpp
HEADERS += mainwindow.h \
    Dialogs/vardialog.h \
    Dialogs/stepresponsedialog.h \
    Dialogs/bodedialog.h \
    Dialogs/expressiondialog.h \
    expressionmodel.h \
    varmodel.h \
    Dialogs/rangeselectordialog.h \
    Dialogs/parametersliderdialog.h \
    numericallaplace.h \
    curvemodel.h \
    definitions.h \
    mathFunction/mathfunctionevaluator.h \
    mathFunction/mathfunctioncompiler.h \
    colorlisteditor.h \
    controlexpression.h \
    csv.h \
    Dialogs/csvdialog.h \
    Curves/qwt_root_locus_curve.h \
    Curves/qwt_response_curve.h \
    Curves/qwt_control_plot_item.h \
    Curves/qwt_zeta_curve.h \
    Curves/qwt_dataset_curve.h \
    Curves/qwt_magnitude_curve.h \
    Curves/qwt_phase_curve.h \
    Curves/qwt_curve_fitter.h \
    Dialogs/expressionclonedialog.h \
    Curves/qwt_discrete_response_curve.h
FORMS += mainwindow.ui \
    Dialogs/vardialog.ui \
    Dialogs/bodedialog.ui \
    Dialogs/expressiondialog.ui \
    Dialogs/stepresponsedialog.ui \
    Dialogs/rangeselectordialog.ui \
    Dialogs/parametersliderdialog.ui \
    Dialogs/csvdialog.ui \
    Dialogs/expressionclonedialog.ui
INCLUDEPATH += /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4 \
    Curves \
    muparserx_v2_1_6/parser \
    Dialogs
LIBS += -L/usr/lib/ \
    -L. \
    -L muparserx_v2_1_6/bin \
    -lmuparserx \
    -lqwt \
    -lm \
    -lginac

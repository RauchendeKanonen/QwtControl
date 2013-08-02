# -------------------------------------------------
# Project created by QtCreator 2012-10-02T14:42:15
# -------------------------------------------------
#QMAKE_CXXFLAGS += -ffast-math -O2 -ftree-vectorize -msse2 -ftree-vectorizer-verbose=5
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
    Dialogs/rangeselectordialog.cpp \
    Dialogs/parametersliderdialog.cpp \
    mathFunction/mathfunctionevaluator.cpp \
    mathFunction/mathfunctioncompiler.cpp \
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
    Curves/qwt_discrete_response_curve.cpp \
    Curves/qwt_numeric_root_locus_curve.cpp \
    numeric/newton.cpp \
    util/tools.cpp \
    Dialogs/systemdialog.cpp \
    Curves/qwt_z_zeta_curve.cpp \
    numeric/numericallaplace.cpp \
    models/varmodel.cpp \
    models/expressionmodel.cpp \
    models/curvemodel.cpp \
    util/csv.cpp \
    Curves/qwt_z_wd_curve.cpp \
    Dialogs/closeloopdialog.cpp \
    numeric/numericallaplace_weeks.cpp \
    numeric/numericallaplace_gaverstehfest.cpp \
    Dialogs/weeksetupdialog.cpp \
    Dialogs/discretesystemdialog.cpp \
    Dialogs/controlsystemtracker.cpp \
    Dialogs/texteditdialog.cpp \
    Dialogs/colorlisteditor.cpp \
    mathFunction/controlexpression.cpp
HEADERS += mainwindow.h \
    Dialogs/vardialog.h \
    Dialogs/stepresponsedialog.h \
    Dialogs/bodedialog.h \
    Dialogs/expressiondialog.h \
    Dialogs/rangeselectordialog.h \
    Dialogs/parametersliderdialog.h \
    definitions.h \
    mathFunction/mathfunctionevaluator.h \
    mathFunction/mathfunctioncompiler.h \
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
    Curves/qwt_discrete_response_curve.h \
    Curves/qwt_numeric_root_locus_curve.h \
    numeric/newton.h \
    util/tools.h \
    Dialogs/systemdialog.h \
    Curves/qwt_z_zeta_curve.h \
    numeric/numericallaplace.h \
    models/varmodel.h \
    models/expressionmodel.h \
    models/curvemodel.h \
    util/csv.h \
    Curves/qwt_z_wd_curve.h \
    Dialogs/closeloopdialog.h \
    Dialogs/weeksetupdialog.h \
    Dialogs/discretesystemdialog.h \
    Dialogs/controlsystemtracker.h \
    Dialogs/texteditdialog.h \
    Dialogs/colorlisteditor.h \
    mathFunction/controlexpression.h
FORMS += mainwindow.ui \
    Dialogs/vardialog.ui \
    Dialogs/bodedialog.ui \
    Dialogs/expressiondialog.ui \
    Dialogs/stepresponsedialog.ui \
    Dialogs/rangeselectordialog.ui \
    Dialogs/parametersliderdialog.ui \
    Dialogs/csvdialog.ui \
    Dialogs/expressionclonedialog.ui \
    Dialogs/systemdialog.ui \
    Dialogs/closeloopdialog.ui \
    Dialogs/weeksetupdialog.ui \
    Dialogs/discretesystemdialog.ui \
    Dialogs/controlsystemtracker.ui \
    Dialogs/texteditdialog.ui
INCLUDEPATH += /usr/include/qwt5/ \
    /usr/include/qwt-qt4/ \
    /usr/include/qt4 \
    mathFunction \
    Curves \
    muparserx_v2_1_6/parser \
    Dialogs \
    models \
    util \
    numeric
LIBS += -L/usr/lib/ \
    -L. \
    -L muparserx_v2_1_6/bin \
    -lmuparserx \
    -lqwt \
    -lm \
    -lginac

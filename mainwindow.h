#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>


#include <QMainWindow>
#include <QAbstractItemModel>
#include <QStringList>
#include <QStringListModel>
#include "expressiondialog.h"
#include "vardialog.h"
#include "expressionmodel.h"
#include "varmodel.h"
#include <qwt_plot_curve.h>
#include "rangeselectordialog.h"
#include "parametersliderdialog.h"
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <QSemaphore>
#include "bodedialog.h"
#include "stepresponsedialog.h"
#include "QFileDialog"
#include <QMessageBox>
#include "curvemodel.h"
#include "qwt_plot_scaleitem.h"
#include "csv.h"
#include "qwt_root_locus_curve.h"
#include "qwt_zeta_curve.h"
#include "qwt_z_zeta_curve.h"
#include <QPair>
#include "qwt_magnitude_curve.h"
#include "qwt_discrete_response_curve.h"
#include "qwt_numeric_root_locus_curve.h"
#include "qwt_phase_curve.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_panner.h"
#include "qwt_z_wd_curve.h"
#include "Dialogs/closeloopdialog.h"
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void valueChangeSignal(QPair<QString,double> MarkerPair, bool Restart);
    void markerChangeSignal(QPair<QString,double>);
public:

    void deleteVariable(QString Name);
    void insertSlider(QString VariableName, QPointF Range);
    void parameterInit(QString VarName, double DblVal);
    void enqueueCurve(QwtControlPlotItem *Item, QwtPlot *Plot);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void insertVariable(QString Definition);
public slots:
    void rlZoomerSelected(QwtDoubleRect Rect);
    void emitAllValues(void);
    void timerEvent(void);
    void markerPress(void);
    void markerRelease(void);
    void parameterChange(QString VarName, double DblVal);
    void markerChange(QString VarName, double DblVal);
private slots:
    void on_actionExpression_triggered();

    void on_actionVariable_triggered();

    void on_ExpressionListView_customContextMenuRequested(const QPoint &pos);

    void on_VariableListView_customContextMenuRequested(const QPoint &pos);

    void on_ExpressionListView_doubleClicked(const QModelIndex &index);

    void on_VariableListView_doubleClicked(const QModelIndex &index);

    void on_actionLoad_triggered();

    void on_actionSave_as_triggered();

    void on_CurveListView_customContextMenuRequested(const QPoint &pos);

    void on_CurveListView_doubleClicked(const QModelIndex &index);

    void on_CurveListView_clicked(const QModelIndex &index);

    void on_actionSave_triggered();

    void on_actionCurve_from_DataSet_triggered();


    void on_actionResponse_Plot_triggered();

    void on_actionAutoscale_triggered();

    void on_actionSystem_triggered();

    void on_actionClosedLoop_System_triggered();

private:
    Ui::MainWindow *ui;
    ExpressionModel *ExpressionMdl;
    VarModel *VariabelMdl;
    CurveModel *CurveMdl;
    QTimer *UpdateTimer;
    QList <QwtControlPlotItem*> CurveList;
    bool ProjectChanged;
    ParameterSliderDialog *VariableSliderDialog;
    ParameterSliderDialog *IndependentMarkerSliderDialog;
    QString WorkFile;
    QwtZetaCurve *ZetaCurve;
    QwtZZetaCurve *ZZetaCurve;
    QwtZWdCurve  *ZWdCurve;
    QwtPlotZoomer *d_zoomer;
    void deleteAllCurves(void);
    void insertExpression(QString Definition);
    QStringList load(QString FilePath);
    bool store(QString FilePath, QStringList List);
};



#endif // MAINWINDOW_H

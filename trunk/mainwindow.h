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
#include "curvethread.h"
#include "curvesegmentthread.h"
#include "curvemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    void deleteCurve(CurveInformationStruct *CurveInfo);
    void load(QString FilePath);
    QList <CurveInformationStruct*> *dependingCurves(QString VarName);
    bool updateExpressionVars(ParserX *Expression, QString IndependentVars);
    QStringList independentVariables(void);
    explicit MainWindow(QWidget *parent = 0);
    void initCurveInformationStruct(QwtPlot *Plot, ParserX *Expression, ParserX *BaseExpression, double StartPoint, double EndPoint, double Resolution, int CurveType, QString IndependentVar, QwtPlotCurve *Curve);
    ~MainWindow();
    void store(QString FilePath);
    double independentValue(QString IndepName, double *IndepVal);
    void closeEvent(QCloseEvent *event);
public slots:
    void CurveSegmentReady(CurveInformationStruct *CurveInfo);
    void CurveReady(CurveInformationStruct *CurveInfo);
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

private:
    Ui::MainWindow *ui;
    ExpressionModel *ExpressionMdl;
    VarModel *VariabelMdl;
    CurveModel *CurveMdl;
    QList <CurveInformationStruct*> CurveInformationList;

    ParameterSliderDialog *VariableSliderDialog;
    ParameterSliderDialog *IndependentMarkerSliderDialog;
    QString WorkFile;
};

#endif // MAINWINDOW_H

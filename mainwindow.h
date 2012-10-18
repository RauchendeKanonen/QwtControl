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
#include <QPair>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void valueChangeSignal(QPair<QString,double>);
    void markerChangeSignal(QPair<QString,double>);
public:
    void emitAllValues(void);
    void load(QString FilePath);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void store(QString FilePath);
    void closeEvent(QCloseEvent *event);
public slots:
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


private:
    Ui::MainWindow *ui;
    ExpressionModel *ExpressionMdl;
    VarModel *VariabelMdl;
    CurveModel *CurveMdl;

    QList <QwtPlotItem*> CurveList;

    ParameterSliderDialog *VariableSliderDialog;
    ParameterSliderDialog *IndependentMarkerSliderDialog;
    QString WorkFile;
};

#endif // MAINWINDOW_H

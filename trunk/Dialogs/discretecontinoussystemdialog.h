#ifndef DISCRETECONTINOUSSYSTEMDIALOG_H
#define DISCRETECONTINOUSSYSTEMDIALOG_H

#include <QDialog>
#include "mainwindow.h"



namespace Ui {
class DiscreteContinousSystemDialog;
}

class DiscreteContinousSystemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DiscreteContinousSystemDialog(QWidget *parent = 0);
    ~DiscreteContinousSystemDialog();
    QStringList getExpressions(void);
    EvalInfo getEvalInfo(void);
private slots:
    void on_checkBoxShowKernel_clicked();

    void on_doubleSpinBoxDt_valueChanged(double arg1);

    void on_spinBoxDots_valueChanged(int arg1);

private:
    Ui::DiscreteContinousSystemDialog *ui;
    MainWindow *pParent;
    QwtResponseCurve *Kernel;
    StepResponseDialog *StepRespDialog;
    QwtPlotMarker *Marker;
};

#endif // DISCRETECONTINOUSSYSTEMDIALOG_H

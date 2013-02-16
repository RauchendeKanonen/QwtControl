#ifndef WEEKSETUPDIALOG_H
#define WEEKSETUPDIALOG_H

#include <QDialog>

namespace Ui {
class WeekSetupDialog;
}

class WeekSetupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WeekSetupDialog(int NumCoeffWeeks, double ConvergenceAbscissaWeeks, double ContourScaleWeeks, double EvaluationPosWeeks);
    ~WeekSetupDialog();
    void getParms(int *NumCoeffWeeks, double *ConvergenceAbscissaWeeks, double *ContourScaleWeeks, double *EvaluationPosWeeks);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::WeekSetupDialog *ui;

};

#endif // WEEKSETUPDIALOG_H

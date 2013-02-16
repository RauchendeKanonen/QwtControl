#include "weeksetupdialog.h"
#include "ui_weeksetupdialog.h"

WeekSetupDialog::WeekSetupDialog(int NumCoeffWeeks, double ConvergenceAbscissaWeeks, double ContourScaleWeeks, double EvaluationPosWeeks) :
    QDialog(NULL),
    ui(new Ui::WeekSetupDialog)
{
    ui->setupUi(this);
    ui->spinBoxCoeffp2->setValue(NumCoeffWeeks);
    ui->doubleSpinBoxAbscissica->setValue(ConvergenceAbscissaWeeks);
    ui->doubleSpinBoxScaleParm->setValue(ContourScaleWeeks);
    ui->doubleSpinBoxEvaluationPos->setValue(EvaluationPosWeeks);
}

WeekSetupDialog::~WeekSetupDialog()
{
    delete ui;
}

void WeekSetupDialog::on_buttonBox_accepted()
{

}

void WeekSetupDialog::getParms(int *NumCoeffWeeks, double *ConvergenceAbscissaWeeks, double *ContourScaleWeeks, double *EvaluationPosWeeks)
{
    *NumCoeffWeeks = ui->spinBoxCoeffp2->value();
    *ConvergenceAbscissaWeeks = ui->doubleSpinBoxAbscissica->value();
    *ContourScaleWeeks = ui->doubleSpinBoxScaleParm->value();
    *EvaluationPosWeeks = ui->doubleSpinBoxEvaluationPos->value();
}

#include "stepresponsedialog.h"
#include "ui_stepresponsedialog.h"
#include "mainwindow.h"
StepResponseDialog::StepResponseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepResponseDialog)
{
    pParent = parent;
    ui->setupUi(this);
    ResponseCurve = new QwtPlotControlCurve( "Amplidue" );
    ResponseCurve->attach(ui->qwtPlot);
}

StepResponseDialog::~StepResponseDialog()
{
    delete ui;
}

QwtPlotControlCurve *StepResponseDialog::getPlotCurve(void)
{
    return ResponseCurve;
}

QwtPlot *StepResponseDialog::getPlot(void)
{
    return ui->qwtPlot;
}

void StepResponseDialog::setResponse(const double *XData, const double *YData, int Size)
{
    ResponseCurve->setData((const double*)XData,(const double*) YData, Size);
}

void StepResponseDialog::replot(void)
{
    ui->qwtPlot->replot();
}

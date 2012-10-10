#include "stepresponsedialog.h"
#include "ui_stepresponsedialog.h"
#include "mainwindow.h"
StepResponseDialog::StepResponseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepResponseDialog)
{
    pParent = parent;
    ui->setupUi(this);
    ResponseCurve = new QwtPlotCurve( "Amplidue" );
    ResponseCurve->attach(ui->qwtPlot);
}

StepResponseDialog::~StepResponseDialog()
{
    delete ui;
}

QwtPlotCurve *StepResponseDialog::getPlotCurve(void)
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

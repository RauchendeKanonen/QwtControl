#include "bodedialog.h"
#include "ui_bodedialog.h"
#include "mainwindow.h"



BodeDialog::BodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BodeDialog)
{
    ui->setupUi(this);
    pParent = parent;
    ui->qwtAmplitudePlot->setAxisScaleEngine( QwtPlot::yRight, new QwtLog10ScaleEngine );
    ui->qwtAmplitudePlot->setAxisScaleEngine( QwtPlot::xBottom, new QwtLog10ScaleEngine );
    ui->qwtPhasePlot->setAxisScaleEngine( QwtPlot::xBottom, new QwtLog10ScaleEngine );
    ui->qwtPhasePlot->setAxisScaleEngine( QwtPlot::yRight, new QwtLinearScaleEngine );
}

BodeDialog::~BodeDialog()
{
    delete ui;
}

QwtPlot *BodeDialog::getPhasePlot(void)
{
    return ui->qwtPhasePlot;
}


QwtPlot *BodeDialog::getAmplitudePlot(void)
{
    return ui->qwtAmplitudePlot;
}

void BodeDialog::replot(void)
{
    ui->qwtPhasePlot->replot();
    ui->qwtAmplitudePlot->replot();
}






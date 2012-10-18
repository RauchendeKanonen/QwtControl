#include "stepresponsedialog.h"
#include "ui_stepresponsedialog.h"
#include "mainwindow.h"
StepResponseDialog::StepResponseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepResponseDialog)
{
    pParent = parent;
    ui->setupUi(this);


    setAcceptDrops(true);
}

void StepResponseDialog::dragMoveEvent(QDragMoveEvent *de)
{
    // The event needs to be accepted here
    de->accept();
}

void StepResponseDialog::dragEnterEvent(QDragEnterEvent *event)
{
    // Set the drop action to be the proposed action.
    event->acceptProposedAction();
}

void StepResponseDialog::dropEvent ( QDropEvent * event )
{
//    QByteArray arraydata = event->mimeData()->data("application/set-curve");
//    CurveInformationStruct **CurveInfo = (CurveInformationStruct**)arraydata.constData();
//    (*CurveInfo)->Plot = ui->qwtPlot;
//    (*CurveInfo)->Curve->attach(ui->qwtPlot);
//    ui->qwtPlot->replot();
}

StepResponseDialog::~StepResponseDialog()
{
    delete ui;
}


QwtPlot *StepResponseDialog::getPlot(void)
{
    return ui->qwtPlot;
}

void StepResponseDialog::replot(void)
{
    ui->qwtPlot->replot();
}

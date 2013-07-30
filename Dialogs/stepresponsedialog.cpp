#include "stepresponsedialog.h"
#include "ui_stepresponsedialog.h"
#include "mainwindow.h"
StepResponseDialog::StepResponseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepResponseDialog)
{
    pParent = parent;
    ui->setupUi(this);
    setWindowTitle("Response");
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode( QwtLegend::ClickableItem );
    ui->qwtPlot->insertLegend( legend, QwtPlot::RightLegend );

    connect( ui->qwtPlot, SIGNAL(legendClicked(QwtPlotItem*)),
             this, SLOT( legendClicked( QwtPlotItem *) ) );



    setAcceptDrops(true);
}

void StepResponseDialog::legendClicked(QwtPlotItem *)
{



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
    if(event->mimeData()->hasFormat("application/set-curve"))
    {
        QByteArray arraydata = event->mimeData()->data("application/set-curve");
        QwtControlPlotItem **CurveInfo = (QwtControlPlotItem**)arraydata.constData();
        (*CurveInfo)->attach(ui->qwtPlot);
        ui->qwtPlot->replot();
    }

    if(event->mimeData()->hasFormat("application/x-expression"))
    {
        QByteArray arraydata = event->mimeData()->data("application/x-expression");
        ControlExpression **ppExpression = (ControlExpression**)arraydata.constData();

        RangeSelectorDialog Dlg(this, QString());
        RedoDlg:
        Dlg.setModal(true);
        if(!Dlg.exec())
            return;


        QPointF Range = Dlg.getRange();
        if(Range.x() >= Range.y())
        {
            QMessageBox Box;
            Box.setText(QString("Min/Max incorrect!!"));
            Box.setModal(true);
            Box.exec();
            goto RedoDlg;
        }

        QString VarName = Dlg.getVarName();
        if(VarName.count() == 0)//|| VariabelMdl->getVarValuePtr(VarName) == NULL)
        {
            QMessageBox Box;
            Box.setText(QString("Variabel Name incorrect!!"));
            Box.setModal(true);
            Box.exec();
            goto RedoDlg;
        }
        double Resolution = Dlg.getResolution();
        QColor Color = Dlg.getColor();


        QString FunctionName = (*ppExpression)->getExpressionName();
        ControlExpression *Expression = *ppExpression;

        Expression->setIndependentVarName(VarName);

        if(FunctionName == QString())
        {
            QMessageBox Box;
            Box.setText(QString("Unnamed Functions are not allowed!!!"));
            Box.setModal(true);
            Box.exec();
            return;
        }
        //add the marker to the variables
        /*if(NULL == VariabelMdl->getVarValuePtr(VarName))
        {
            double Val = 0;
            QString ValueString;
            ValueString.sprintf("%f", Val);
            QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
            index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->setData(index, QString(VarName + QString(" = ") + ValueString),Qt::EditRole);
        }*/


        EvalInfo Evinfo;
        Evinfo.IndepStart = Range.x();
        Evinfo.IndepEnd = Range.y();
        Evinfo.Resolution = Resolution;

        QwtResponseCurve *Curve = new QwtResponseCurve(Expression, Evinfo);


        Curve->attach(getPlot());
        Curve->setPen(QPen(Color));
        Curve->start();

        ((MainWindow*)pParent)->enqueueCurve(Curve, ui->qwtPlot);
        ui->qwtPlot->replot();
    }

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

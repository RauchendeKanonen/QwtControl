#include "discretecontinoussystemdialog.h"
#include "ui_discretecontinoussystemdialog.h"
#include "Dialogs/templatedialog.h"


DiscreteContinousSystemDialog::DiscreteContinousSystemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiscreteContinousSystemDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Discrete Continous System");
    pParent = (MainWindow*)parent;
    Kernel = NULL;
    StepRespDialog = NULL;

    QColor Color("blue");
    Marker = new QwtPlotMarker();
    Marker->setSymbol( QwtSymbol( QwtSymbol::XCross, QColor(Color), QColor(Color), QSize( 15, 15 )));
}

DiscreteContinousSystemDialog::~DiscreteContinousSystemDialog()
{

    if(Kernel)
    {
        pParent->curveModel()->removeCurve(Kernel);
        QwtPlot *Plot = Kernel->plot();
        Kernel->detach();
        Kernel->stopThread();
        delete Kernel;

    }

    Marker->detach();
    delete Marker;

    if(StepRespDialog)
    {
        delete StepRespDialog;
    }

    delete ui;
}

void DiscreteContinousSystemDialog::on_checkBoxShowKernel_clicked()
{
    EvalInfo Evinfo;
    Evinfo.IndepStart = 0;
    Evinfo.IndepEnd = ui->doubleSpinBoxDt->value()*ui->spinBoxDots->value()*2.0;
    Evinfo.Resolution = ui->doubleSpinBoxDt->value();

    if(Kernel)
    {
        pParent->curveModel()->removeCurve(Kernel);
        QwtPlot *Plot = Kernel->plot();
        Kernel->detach();
        Kernel->stopThread();
        delete Kernel;
        if(Plot)
            Plot->replot();
    }
    if(StepRespDialog == NULL)
    {
        StepRespDialog = new StepResponseDialog(this);
        StepRespDialog->show();
    }

    ControlExpression *pExp = pParent->expressionModel()->createExpression(ui->lineEditLSystem->text(), QString("s"));

    if(pExp == NULL)
    {
        QMessageBox Box;
        Box.setText("Expression not found!");
        Box.setModal(true);
        Box.exec();
        return;
    }

    try
    {
        Kernel = new QwtResponseCurve(pExp, Evinfo);
        qDebug("%d", (long int)Kernel);
    }
    catch(const QString &e)
    {
        QMessageBox Box;
        Box.setText(e);
        Box.setModal(true);
        Box.exec();
        return;
    }

    pParent->enqueueCurve(Kernel, StepRespDialog->getPlot());

    Kernel->setPen(QPen(QColor("red")));
}

QStringList DiscreteContinousSystemDialog::getExpressions(void)
{
    QStringList Expressions;
    if(pParent->expressionModel()->getExpression(ui->lineEditZController->text()).count())
        Expressions.append(pParent->expressionModel()->getExpression(ui->lineEditZController->text()));
    else
        Expressions.append(ui->lineEditZController->text());


    if(pParent->expressionModel()->getExpression(ui->lineEditLSystem->text()).count())
        Expressions.append(pParent->expressionModel()->getExpression(ui->lineEditLSystem->text()));
    else
        Expressions.append(ui->lineEditLSystem->text());


    return Expressions;
}

EvalInfo DiscreteContinousSystemDialog::getEvalInfo(void)
{
    EvalInfo Evinfo;
    Evinfo.IndepStart = 0;
    Evinfo.IndepEnd = ui->doubleSpinBoxDt->value()*ui->spinBoxDots->value()*2.0;
    Evinfo.Resolution = ui->doubleSpinBoxDt->value();
    Evinfo.Dots = Evinfo.IndepEnd/Evinfo.Resolution;
    return Evinfo;
}


void DiscreteContinousSystemDialog::on_doubleSpinBoxDt_valueChanged(double arg1)
{

    double KernelSampling = ui->doubleSpinBoxDt->value()*ui->spinBoxDots->value();

    if(Kernel == 0)
        return;

    if(StepRespDialog)
    {
        QwtPlot *Plot = Kernel->plot();

        Marker->setXValue(KernelSampling);
        Marker->setYValue(Kernel->data().y(ui->spinBoxDots->value()));
        Marker->attach(Plot);
        Plot->replot();
    }
}
void DiscreteContinousSystemDialog::on_spinBoxDots_valueChanged(int arg1)
{

    double KernelSampling = ui->doubleSpinBoxDt->value()*ui->spinBoxDots->value();

    if(Kernel)
    {
        pParent->curveModel()->removeCurve(Kernel);
        QwtPlot *Plot = Kernel->plot();
        Kernel->detach();
        Kernel->stopThread();
        delete Kernel;
    }
    EvalInfo Evinfo;
    Evinfo.IndepStart = 0;
    Evinfo.IndepEnd = ui->doubleSpinBoxDt->value()*ui->spinBoxDots->value()*2.0;
    Evinfo.Resolution = ui->doubleSpinBoxDt->value();


    ControlExpression *pExp = pParent->expressionModel()->createExpression(ui->lineEditLSystem->text(), QString("s"));

    if(pExp == NULL)
    {
        QMessageBox Box;
        Box.setText("Expression not found!");
        Box.setModal(true);
        Box.exec();
        return;
    }

    Kernel = new QwtResponseCurve(pExp, Evinfo);

    if(StepRespDialog)
    {
        pParent->enqueueCurve(Kernel, StepRespDialog->getPlot());

        Kernel->setPen(QPen(QColor("red")));


        QwtPlot *Plot = Kernel->plot();

        Marker->setXValue(KernelSampling);
        Marker->setYValue(Kernel->data().y(ui->spinBoxDots->value()));
        Marker->attach(Plot);
        Plot->replot();
    }
}

void DiscreteContinousSystemDialog::on_toolButtonUpdate_clicked()
{
    on_checkBoxShowKernel_clicked();
}

void DiscreteContinousSystemDialog::on_commandLinkButtonContTemp_clicked()
{
    TemplateDialog DlgTmpl;
    DlgTmpl.setModal(true);
    if(DlgTmpl.exec())
        ui->lineEditZController->setText(DlgTmpl.getExpression());
}

void DiscreteContinousSystemDialog::on_commandLinkButtonSysTemp_clicked()
{
    TemplateDialog DlgTmpl;
    DlgTmpl.setModal(true);
    if(DlgTmpl.exec())
        ui->lineEditLSystem->setText(DlgTmpl.getExpression());
}

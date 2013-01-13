#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModelIndex>
#include "expressionmodel.h"
#include "varmodel.h"
#include "numericallaplace.h"
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "controlexpression.h"
#include "csvdialog.h"
#include "QTextStream"
#include "qwt_dataset_curve.h"
#include "expressionclonedialog.h"
#include <qwt_data.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ExpressionMdl = new ExpressionModel();
    ui->ExpressionListView->setModel(ExpressionMdl);
    VariabelMdl = new VarModel();
    ui->VariableListView->setModel(VariabelMdl);

    CurveMdl = new CurveModel();
    ui->CurveListView->setModel(CurveMdl);
    CurveMdl->setCurveList(&CurveList);


    ui->CurveListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->CurveListView->setDragEnabled(true);
    ui->CurveListView->setAcceptDrops(true);
    ui->CurveListView->setDropIndicatorShown(true);


    ui->ExpressionListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ExpressionListView->setDragEnabled(true);
    ui->ExpressionListView->setAcceptDrops(true);
    ui->ExpressionListView->setDropIndicatorShown(true);


    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft);

    QwtPlotScaleItem *scaleItem = new QwtPlotScaleItem(QwtScaleDraw::TopScale, 0.0);
    scaleItem->attach(ui->qwtPlot);
    scaleItem = new QwtPlotScaleItem(QwtScaleDraw::RightScale, 0.0);
    scaleItem->attach(ui->qwtPlot);
    ui->qwtPlot->enableAxis(QwtPlot::yLeft, false);
    ui->qwtPlot->enableAxis(QwtPlot::xBottom, false);

    ui->ExpressionListView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->VariableListView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->CurveListView->setContextMenuPolicy(Qt::CustomContextMenu);



    VariableSliderDialog = new ParameterSliderDialog(this);
    VariableSliderDialog->setWindowTitle(QString("Variable Sliders"));
    connect(VariableSliderDialog, SIGNAL(parameterChange(QString , double )), this, SLOT(parameterChange(QString , double )));
    IndependentMarkerSliderDialog = new ParameterSliderDialog(this);
    IndependentMarkerSliderDialog->setWindowTitle(QString("Independent Sliders"));
    connect(IndependentMarkerSliderDialog, SIGNAL(parameterChange(QString , double )), this, SLOT(markerChange(QString , double )));

    setWindowTitle(QString("untittled"));

    insertVariable(QString("zeta = 0.5"));
    ZetaCurve = new QwtZetaCurve();
    ZetaCurve->enable(true);
    ZetaCurve->setVisible(false);
    ZetaCurve->attach(ui->qwtPlot);
    enqueueCurve(ZetaCurve);


    d_zoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft,  ui->qwtPlot->canvas());
    d_zoomer->setTrackerMode(QwtPicker::AlwaysOn);
    d_zoomer->setTrackerPen(QColor(Qt::black));
    d_zoomer->setResizeMode(QwtPlotZoomer::KeepSize);
    d_zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::NoButton, Qt::ControlModifier);

    QwtPlotPanner *Panner = new QwtPlotPanner(ui->qwtPlot->canvas());
    Panner->setAxisEnabled(QwtPlot::yRight, false);
    Panner->setMouseButton(Qt::RightButton);
    connect(Panner, SIGNAL(panned(int, int)), ui->qwtPlot, SIGNAL(panned(int, int)));

    QwtPlotMagnifier *Magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
    Magnifier->setMouseButton(Qt::RightButton, Qt::NoButton);
    Magnifier->setMouseButton(Qt::LeftButton, Qt::NoButton);
    Magnifier->setMouseButton(Qt::MidButton, Qt::NoButton);

    UpdateTimer = new QTimer(this);
    UpdateTimer->setInterval(100);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    ProjectChanged = false;
}


void MainWindow::markerPress(void)
{
    UpdateTimer->start();
}

void MainWindow::timerEvent(void)
{
    ui->qwtPlot->replot();
}

void MainWindow::markerRelease(void)
{
    UpdateTimer->stop();
    UpdateTimer->singleShot(500, this, SLOT(timerEvent()));
}


MainWindow::~MainWindow()
{
    delete VariabelMdl;
    delete ExpressionMdl;
    delete CurveMdl;
    delete ui;
    if(VariableSliderDialog)
        delete VariableSliderDialog;
    if(IndependentMarkerSliderDialog)
        delete IndependentMarkerSliderDialog;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ProjectChanged != false)
    {
        int r = QMessageBox::question(this, QString("Wirklich beenden?"), QString("Alle daten des bestehenden ControlSystems gehen verloren"),
        QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
        if(r == QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }
    event = event;
    deleteAllCurves();
    delete VariableSliderDialog;
    VariableSliderDialog = NULL;
    delete IndependentMarkerSliderDialog;
    IndependentMarkerSliderDialog = NULL;
}


void MainWindow::on_actionExpression_triggered()
{
    ProjectChanged = true;
    QString Expression;
    ExpressionDialog Dlg(this, &Expression);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;

    insertExpression(Expression);
}



void MainWindow::on_actionVariable_triggered()
{
    ProjectChanged = true;
    double Val;
    QString Name;
    VarDialog Dlg(this, &Val, &Name);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;

    QString ValueString;
    ValueString.sprintf("%f", Val);
    insertVariable(QString(Name + QString(" = ") + ValueString));

}
void MainWindow::insertExpression(QString Expression)
{
    ProjectChanged = true;
    QModelIndex index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->setData(index, Expression,Qt::EditRole);
}

void MainWindow::insertVariable(QString Definition)
{
    ProjectChanged = true;
    QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->setData(index, Definition,Qt::EditRole);
}

void MainWindow::enqueueCurve(QwtZetaCurve *Item)
{
    connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Item, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
    connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Item, SLOT(markerChangeSlot(QPair<QString,double>)));
    CurveList.append(Item);
    CurveMdl->valueChange();
}

void MainWindow::enqueueCurve(QwtResponseCurve *Item)
{
    connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Item, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
    connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Item, SLOT(markerChangeSlot(QPair<QString,double>)));
    CurveList.append(Item);
    CurveMdl->valueChange();
}

void MainWindow::enqueueCurve(QwtPhaseCurve *Item)
{
    connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Item, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
    connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Item, SLOT(markerChangeSlot(QPair<QString,double>)));
    CurveList.append(Item);
    CurveMdl->valueChange();
}

void MainWindow::enqueueCurve(QwtMagnitudeCurve *Item)
{
    connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Item, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
    connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Item, SLOT(markerChangeSlot(QPair<QString,double>)));
    CurveList.append(Item);
    CurveMdl->valueChange();
}

void MainWindow::on_ExpressionListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->ExpressionListView->mapToGlobal(pos); // Map the global position to the userlist
    QModelIndex Index = ui->ExpressionListView->indexAt(pos);
    if(!Index.isValid())
        return;

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("draw Root Locus"));
    ExpressionMenu.addAction(QString("draw Bode"));
    ExpressionMenu.addAction(QString("draw Response"));
    ExpressionMenu.addAction(QString("clone and substitute"));
    ExpressionMenu.addAction(QString("draw discrete Response"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    if(happened->text() == QString("clone and substitute"))
    {
        ExpressionCloneDialog Dlg(this, ExpressionMdl->getExpressionDefinition(Index));
        Dlg.setModal(true);
        if(Dlg.exec())
           this->insertExpression(Dlg.getNewExpression());
    }

    ControlExpression *preExpression = ExpressionMdl->createExpression(Index, QString());

    RangeSelectorDialog Dlg(this, preExpression->getVariables());
    delete preExpression;

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

    if(happened->text() == QString("draw Root Locus"))
    {

        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        ControlExpression *Expression = ExpressionMdl->createExpression(Index, VarName);

        if(Expression->getImagEvaluator() == NULL || Expression->getRealEvaluator() == NULL)
        {
            QMessageBox Box;
            Box.setText(QString("Could not compile Function c-code!!!"));
            Box.setModal(true);
            Box.exec();
            return;
        }


        if(FunctionName == QString())
        {
            QMessageBox Box;
            Box.setText(QString("Unnamed Functions are not allowed!!!"));
            Box.setModal(true);
            Box.exec();
            return;
        }
        //add the marker to the variables
        if(NULL == VariabelMdl->getVarValuePtr(VarName))
        {
            double Val = 0;
            QString ValueString;
            ValueString.sprintf("%f", Val);
            QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
            index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->setData(index, QString(VarName + QString(" = ") + ValueString),Qt::EditRole);
        }


        EvalInfo Evinfo;
        Evinfo.IndepStart = Range.x();
        Evinfo.IndepEnd = Range.y();
        Evinfo.Resolution = Resolution;

        QwtRootLocusCurve *Curve = new QwtRootLocusCurve(Expression, Evinfo);

        Curve->setSymbol(QwtSymbol());
        connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Curve, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
        connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Curve, SLOT(markerChangeSlot(QPair<QString,double>)));
        Curve->attach(ui->qwtPlot);
        Curve->setPen(QPen(Color));
        //Curve->setSymbol(QwtSymbol( QwtSymbol::Rect,
                      //                        QColor(Color), QColor(Color), QSize( 2, 2 ) ));
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        emitAllValues();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        Curve->start();

        CurveList.append((QwtRootLocusCurve*)Curve);

        IndependentMarkerSliderDialog->addSlider(VarName, Range);
        IndependentMarkerSliderDialog->show();
        IndependentMarkerSliderDialog->setVisible(true);
        IndependentMarkerSliderDialog->setFocus();
        IndependentMarkerSliderDialog->activateWindow();
        IndependentMarkerSliderDialog->raise();
    }


    if(happened->text() == QString("draw Bode"))
    {
        BodeDialog *BodeDiagram;
        BodeDiagram = new BodeDialog(this);
        BodeDiagram->show();

        QwtPlot *MagnitudePlot = BodeDiagram->getAmplitudePlot();
        QwtPlot *PhasePlot = BodeDiagram->getPhasePlot();


        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        ControlExpression *MagExpression = ExpressionMdl->createExpression(Index, VarName);
        ControlExpression *PhaExpression = ExpressionMdl->createExpression(Index, VarName);
        EvalInfo Evinfo;
        Evinfo.IndepStart = Range.x();
        Evinfo.IndepEnd = Range.y();
        Evinfo.Resolution = Resolution;

        QwtMagnitudeCurve *MagCurve = new QwtMagnitudeCurve(MagExpression, Evinfo);
        MagCurve->attach(MagnitudePlot);
        emitAllValues();
        MagCurve->start();
        MagnitudePlot->replot();

        QwtPhaseCurve *PhaCurve = new QwtPhaseCurve(PhaExpression, Evinfo);
        PhaCurve->attach(PhasePlot);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        emitAllValues();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        PhaCurve->start();
        PhasePlot->replot();
        enqueueCurve(PhaCurve);
        enqueueCurve(MagCurve);

        connect(MagCurve, SIGNAL(amplitudeMarkerChangeSignal(double)), PhaCurve, SLOT(phaseMarkerChangeSlot(double)));


    }

    if(happened->text() == QString("draw Response"))
    {
        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        ControlExpression *Expression = ExpressionMdl->createExpression(Index, VarName);

        if(FunctionName == QString())
        {
            QMessageBox Box;
            Box.setText(QString("Unnamed Functions are not allowed!!!"));
            Box.setModal(true);
            Box.exec();
            return;
        }
        //add the marker to the variables
        if(NULL == VariabelMdl->getVarValuePtr(VarName))
        {
            double Val = 0;
            QString ValueString;
            ValueString.sprintf("%f", Val);
            QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
            index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->setData(index, QString(VarName + QString(" = ") + ValueString),Qt::EditRole);
        }


        EvalInfo Evinfo;
        Evinfo.IndepStart = Range.x();
        Evinfo.IndepEnd = Range.y();
        Evinfo.Resolution = Resolution;

        StepResponseDialog *StepRespDialog = new StepResponseDialog(this);
        StepRespDialog->show();

        QwtResponseCurve *Curve = new QwtResponseCurve(Expression, Evinfo);


        connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Curve, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
        connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Curve, SLOT(markerChangeSlot(QPair<QString,double>)));
        Curve->attach(StepRespDialog->getPlot());
        Curve->setPen(QPen(Color));
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        emitAllValues();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        Curve->start();

        CurveList.append((QwtControlPlotItem*)Curve);
    }
    if(happened->text() == QString("draw discrete Response"))
    {
        StepResponseDialog *StepRespDialog = new StepResponseDialog(this);
        StepRespDialog->show();


        EvalInfo EvInfo;
        EvInfo.Dots = 100;
        QwtDiscreteResponseCurve *Curve = new QwtDiscreteResponseCurve(ExpressionMdl->getExpressionDefinition(Index),EvInfo);
        connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Curve, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
        connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Curve, SLOT(markerChangeSlot(QPair<QString,double>)));
        Curve->attach(StepRespDialog->getPlot());
        Curve->setPen(QPen(Color));
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        emitAllValues();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        Curve->start();

        CurveList.append((QwtControlPlotItem*)Curve);
    }
    CurveMdl->valueChange();
}

void MainWindow::on_VariableListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->VariableListView->mapToGlobal(pos); // Map the global position to the userlist
    QModelIndex t = ui->VariableListView->indexAt(pos);
    if(!t.isValid())
        return;
    QString VariableName = VariabelMdl->getVarName(t);
    QMenu SliderMenu;
    SliderMenu.addAction(QString("add Slider"));
    QAction *happened = SliderMenu.exec(globalPos);

    if(happened == NULL)
        return;

    mapToGlobal(pos);

    if(happened->text() == QString("add Slider"))
    {
        RangeSelectorDialog Dlg(this, VariableName);
        Dlg.setModal(true);
        Dlg.exec();
        QPointF Range = Dlg.getRange();

        double *ActValue = VariabelMdl->getVarValuePtr(VariableName);

        VariableSliderDialog->addSlider(VariableName, Range, *ActValue);
        VariableSliderDialog->show();
        VariableSliderDialog->setVisible(true);
        VariableSliderDialog->setFocus();
        VariableSliderDialog->activateWindow();
        VariableSliderDialog->raise();
    }
}


void MainWindow::emitAllValues(void)
{
    int numVariabels = VariabelMdl->rowCount();

    for(int i = 0 ; i < numVariabels ; i ++ )
    {
        QModelIndex Index = VariabelMdl->index(i);
        QString VarName = VariabelMdl->getVarName(Index);
        double *Val = VariabelMdl->getVarValuePtr(VarName);
        parameterInit(VarName, *Val);
    }
}

void MainWindow::parameterInit(QString VarName, double DblVal)
{
    double *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        *Val = DblVal;
        VariabelMdl->valueChange();
        QPair<QString,double> VarPair;
        VarPair.first = VarName;
        VarPair.second = DblVal;
        IndependentMarkerSliderDialog->setSlider(VarName, DblVal);
        emit valueChangeSignal(VarPair, false);
    }
}

void MainWindow::parameterChange(QString VarName, double DblVal)
{
    double *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        ProjectChanged = true;
        *Val = DblVal;
        VariabelMdl->valueChange();
        QPair<QString,double> VarPair;
        VarPair.first = VarName;
        VarPair.second = DblVal;
        IndependentMarkerSliderDialog->setSlider(VarName, DblVal);
        emit valueChangeSignal(VarPair, true);
    }
}

void MainWindow::markerChange(QString VarName, double DblVal)
{
    double *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        ProjectChanged = true;
        *Val = DblVal;
        VariabelMdl->valueChange();
        QPair<QString,double> VarPair;
        VarPair.first = VarName;
        VarPair.second = DblVal;
        VariableSliderDialog->setSlider(VarName, DblVal);
        emit markerChangeSignal(VarPair);
        emit valueChangeSignal(VarPair, true);
    }
}

void MainWindow::on_ExpressionListView_doubleClicked(const QModelIndex &index)
{
    ProjectChanged = true;
    QString Expression = ExpressionMdl->getExpressionDefinition(index);
    ExpressionDialog Dlg(this, &Expression);
    Dlg.setModal(true);
    Dlg.exec();

    ExpressionMdl->setData(index, QString(Expression),Qt::EditRole);
}

void MainWindow::on_VariableListView_doubleClicked(const QModelIndex &index)
{
    ProjectChanged = true;
    QString Name = VariabelMdl->getVarName(index);
    double *Val = VariabelMdl->getVarValuePtr(Name);

    VarDialog Dlg(this, Val, &Name);
    Dlg.setModal(true);
    Dlg.exec();

    QString ValueString;
    ValueString.sprintf("%f", *Val);
    VariabelMdl->setData(index, QString(Name + QString(" = ") + ValueString),Qt::EditRole);
    QPair <QString, double> Pair;
    Pair.first = Name;
    Pair.second = *Val;
    emit valueChangeSignal(Pair, true);
}




void MainWindow::on_actionSave_triggered()
{
    ProjectChanged = false;
    if(QString("") != WorkFile)
    {
        store(WorkFile, ExpressionMdl->getExpressionStringList());
        store(WorkFile+QString(".var"), VariabelMdl->getVarStringList());
    }
    else
    {
        on_actionSave_as_triggered();
    }
}


void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dlg(this, QString("Laden"),QString("data/"));
    dlg.setModal(true);

    if(ProjectChanged != false)
    {
        int r = QMessageBox::question(this, QString("Wirklich laden?"), QString("Alle daten des bestehenden ControlSystems gehen verloren"),
        QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
        if(r == QMessageBox::No)
        {
            return;
        }
    }



    deleteAllCurves();

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            setWindowTitle(dlg.selectedFiles().at(0));
            QStringList Expressions = load(dlg.selectedFiles().at(0));
            for(int i = 0 ; i < Expressions.count() ; i++ )
                insertExpression(Expressions.at(i));


            QStringList Variables = load(dlg.selectedFiles().at(0)+QString(".var"));
            for(int i = 0 ; i < Variables.count() ; i++ )
                insertVariable(Variables.at(i));

            WorkFile = dlg.selectedFiles().at(0);
            ProjectChanged = false;
        }
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dlg(this, QString("Speichern"),QString("./data/"));
    dlg.setModal(true);

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            ProjectChanged = false;
            setWindowTitle(dlg.selectedFiles().at(0));

            QString Filename = dlg.selectedFiles().at(0);
            setWindowTitle(dlg.selectedFiles().at(0));
            WorkFile = Filename;
            store(WorkFile, ExpressionMdl->getExpressionStringList());
            store(WorkFile+QString(".var"), VariabelMdl->getVarStringList());

        }
    }
}


void MainWindow::deleteAllCurves(void)
{
    for(; CurveList.count() ; )
    {
        QwtControlPlotItem *CurveItem = CurveList.first();
        CurveList.removeOne(CurveItem);
        QwtPlot *Plot = CurveItem->plot();
        CurveItem->detach();
        CurveItem->stopThread();
        delete CurveItem;
    }
}

void MainWindow::on_CurveListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->CurveListView->mapToGlobal(pos); // Map the global position to the userlist
    QModelIndex index = ui->CurveListView->indexAt(pos);
    if(!index.isValid())
        return;
    QwtControlPlotItem *CurveItem = CurveList.at(index.row());

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("delete"));
    ExpressionMenu.addAction(QString("toggle visability"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    if(happened->text() == QString("delete"))
    {
        CurveList.removeOne(CurveItem);
        CurveMdl->valueChange();
        QwtPlot *Plot = CurveItem->plot();
        CurveItem->detach();
        CurveItem->stopThread();
        delete CurveItem;
        if(Plot)
            Plot->replot();
    }

    if(happened->text() == QString("toggle visability"))
    {
        CurveItem->setVisible(!CurveItem->isVisible());
        QwtPlot *Plot = CurveItem->plot();
        if(Plot)
            Plot->replot();
    }

}

void MainWindow::on_CurveListView_doubleClicked(const QModelIndex &index)
{
    QwtControlPlotItem *Curve = CurveList.at(index.row());
    EvalInfo *EvInfo;
    ControlExpression *pExpression;
    pExpression = Curve->getExpressionPtr();
    EvInfo = Curve->getEvalInfoPtr();

    QwtPlot *Plot = Curve->plot();


    RangeSelectorDialog Dlg(this, pExpression->independentVarName());
    Dlg.setRange(QPointF(EvInfo->IndepStart, EvInfo->IndepEnd));
    Dlg.setResolution(EvInfo->Resolution);
    Dlg.setModal(true);
    Dlg.setColor(Curve->getColor());
    if(!Dlg.exec())
        return;


    QPointF Range = Dlg.getRange();
    double Resolution = Dlg.getResolution();
    QColor Color = Dlg.getColor();
    Curve->setColor(Color);
    EvInfo->IndepStart = Range.x();
    EvInfo->IndepEnd = Range.y();
    EvInfo->Resolution = Resolution;
    Plot->replot();
}

void MainWindow::on_CurveListView_clicked(const QModelIndex &index)
{
    QwtPlot *Plot =  CurveList.at(index.row())->plot();
    if(Plot)
    {
        QWidget *Wdg = (QWidget*) Plot->parent();
        if(Wdg)
        {
            Wdg->setVisible(true);
            Wdg->setFocus();
            Wdg->activateWindow();
            Wdg->raise();
        }
    }
}



void MainWindow::on_actionCurve_from_DataSet_triggered()
{
    QFileDialog dlg(this, QString("import"),QString("data/"));
    dlg.setModal(true);
    QString File;

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            setWindowTitle(dlg.selectedFiles().at(0));
            File = dlg.selectedFiles().at(0);
        }
        else
            return;
    }
    csv CSVFile(File);


    CSVDialog Dlg(this, &CSVFile);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;


    CSVFile.readCSV(-1);
    QPolygonF Dataset = CSVFile.getData();

    EvalInfo EvalI;
    EvalI.IndepStart = Dataset.first().x();
    EvalI.IndepEnd   = Dataset.last().x();

    QwtDataSetCurve *Curve = new QwtDataSetCurve(Dataset, EvalI);
    CurveList.append((QwtControlPlotItem*)Curve);
    CurveMdl->valueChange();
}


QStringList MainWindow::load(QString FilePath)
{
    QStringList List;



    QFile fIn(FilePath);
    if (fIn.open(QFile::ReadOnly | QFile::Text))
    {
        ProjectChanged = false;
        QTextStream sIn(&fIn);
        while (!sIn.atEnd())
            List += sIn.readLine();
    }
    else
    {
        std::cerr << "error opening output file\n";
        return QStringList();
    }

    return List;
}

bool MainWindow::store(QString FilePath, QStringList List)
{
    // write data
    QFile fOut(FilePath);

    if (fOut.open(QFile::WriteOnly | QFile::Text))
    {
        ProjectChanged = false;
        QTextStream s(&fOut);
        for (int i = 0; i < List.size(); ++i)
        {
            if(List.at(i).contains("zeta = "))
                continue;
            s << List.at(i) << '\n';
        }
    }
    else
    {
        std::cerr << "error opening output file\n";
        return false;
    }
    fOut.close();
    return true;
}

void MainWindow::on_actionResponse_Plot_triggered()
{
    StepResponseDialog *StepRespDialog = new StepResponseDialog(this);
    StepRespDialog->show();
}

void MainWindow::on_actionAutoscale_triggered()
{
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom);

    delete d_zoomer;
    d_zoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft,  ui->qwtPlot->canvas());
    d_zoomer->setTrackerMode(QwtPicker::AlwaysOn);
    d_zoomer->setTrackerPen(QColor(Qt::black));
    d_zoomer->setResizeMode(QwtPlotZoomer::KeepSize);
    d_zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::NoButton, Qt::ControlModifier);

}
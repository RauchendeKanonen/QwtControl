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
#include "systemdialog.h"
#include <QColor>
#include "discretesystemdialog.h"
#include "controlsystemtracker.h"
#include "tdkernel.h"
#include "Dialogs/discretecontinoussystemdialog.h"
#include <QIcon>
#include "helpselectordialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->qwtPlot->setCanvasBackground(QColor(0x50,0x80,0xf3));
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


    insertVariable(QString("e = 2.71828182846"));
    insertVariable(QString("pi = 3.14159265359"));
    insertVariable(QString("Wd = 3.14159265359"));
    insertVariable(QString("zeta = 0.5"));

    ZetaCurve = new QwtZetaCurve();
    ZetaCurve->enable(true);
    ZetaCurve->setVisible(false);
    enqueueCurve(ZetaCurve, ui->qwtPlot);

    ZZetaCurve = new QwtZZetaCurve();
    ZZetaCurve->enable(true);
    ZZetaCurve->setVisible(false);
    enqueueCurve(ZZetaCurve, ui->qwtPlot);

    ZWdCurve = new QwtZWdCurve();
    ZWdCurve->enable(true);
    ZWdCurve->setVisible(false);
    enqueueCurve(ZWdCurve, ui->qwtPlot);


    d_zoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft,  ui->qwtPlot->canvas());
    d_zoomer->setTrackerMode(QwtPicker::AlwaysOn);
    d_zoomer->setTrackerPen(QColor(Qt::black));
    d_zoomer->setResizeMode(QwtPlotZoomer::KeepSize);
    d_zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::NoButton, Qt::ControlModifier);
    d_zoomer->setZoomBase( true );
    connect(d_zoomer, SIGNAL(selected(QwtDoubleRect)), this, SLOT(rlZoomerSelected(QwtDoubleRect)));


    QwtPlotPanner *Panner = new QwtPlotPanner(ui->qwtPlot->canvas());
    Panner->setAxisEnabled(QwtPlot::yRight, true);
    Panner->setAxisEnabled(QwtPlot::xBottom, true);
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

    HelpDlg = new TextEditDialog();
    HelpDlg->setWindowTitle("Help Dialog");
    ControlSystemDocDlg = new TextEditDialog();
    ControlSystemDocDlg->setWindowTitle("Control System Documentation");

    //clean up temporary files
    system("rm *.png");
    system("rm latex/*.tex");
    system("rm mathFunction/compile/*.c");
    system("rm mathFunction/compile/*.h");
    system("rm mathFunction/compile/*.o");
    system("rm mathFunction/compile/*.so.0.1");

    setWindowIcon(QIcon(QPixmap("Pictures/logo.png")));
}

void  MainWindow::rlZoomerSelected(QwtDoubleRect Rect)
{
    d_zoomer->setZoomBase(Rect);
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
    delete ControlSystemDocDlg;
    delete HelpDlg;
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
    double Val = 0.0;
    QString Name;
    VarDialog Dlg(this, &Val, &Name);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;

    QString ValueString;
    ValueString.sprintf("%f", Val);
    insertVariable(QString(Name + QString(" = ") + ValueString));
}

QModelIndex MainWindow::insertExpression(QString Expression)
{
    ProjectChanged = true;
    QModelIndex index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->setData(index, Expression,Qt::EditRole);
    return index;
}

void MainWindow::deleteVariable(QString Name)
{
    VariabelMdl->deleteVar(Name);
}

void MainWindow::insertVariable(QString Definition)
{
    QString Name;
    Name = Definition.left(Definition.indexOf(" "));


    if(VariabelMdl->getVarValuePtr(Name) != NULL)
        return;

    ProjectChanged = true;
    QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->setData(index, Definition,Qt::EditRole);
}

void MainWindow::enqueueCurve(QwtControlPlotItem *Item, QwtPlot *Plot)
{
    connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>, bool)), Item, SLOT(valueChangeSlot(QPair<QString,double>, bool)));
    connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Item, SLOT(markerChangeSlot(QPair<QString,double>)));
    CurveMdl->appendCurve(Item);
    Item->attach(Plot);
    Plot->replot();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    emitAllValues();
    QCoreApplication::processEvents(QEventLoop::AllEvents);

    usleep(250*1000);
    Item->terminate();
    Item->start();
}

void MainWindow::on_ExpressionListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->ExpressionListView->mapToGlobal(pos); // Map the global position to the userlist
    QModelIndex Index = ui->ExpressionListView->indexAt(pos);
    if(!Index.isValid())
        return;

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("draw Complex"));
    ExpressionMenu.addAction(QString("draw numeric Root Locus"));
    ExpressionMenu.addAction(QString("draw Response"));
    ExpressionMenu.addAction(QString("draw discrete step Response"));
    ExpressionMenu.addAction(QString("draw Bode"));
    ExpressionMenu.addAction(QString("clone and substitute"));
    ExpressionMenu.addAction(QString("delete"));


    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    on_ExpressionListView_customContextMenuRequested(Index, happened->text());
}

void MainWindow::on_ExpressionListView_customContextMenuRequested(QModelIndex Index, QString MenuEntry)
{

    if(MenuEntry == QString("delete"))
    {
        ExpressionMdl->removeRow(Index.row());
        return;
    }

    if(MenuEntry == QString("clone and substitute"))
    {
        ExpressionCloneDialog Dlg(this, ExpressionMdl->getExpressionDefinition(Index));
        Dlg.setModal(true);
        if(Dlg.exec())
           this->insertExpression(Dlg.getNewExpression());
        return;
    }

    ControlExpression *preExpression = ExpressionMdl->createExpression(Index, QString());

    if(preExpression->getState() == false)
    {
        if(MenuEntry != QString("draw discrete step Response"))
        {
            QMessageBox Box;
            Box.setText(QString("Could not parse Expression!!!"));
            Box.setModal(true);
            Box.exec();
            return;
        }
    }

    RangeSelectorDialog *Dlg;

    if(MenuEntry == QString("draw discrete step Response"))
    {
        Dlg = new RangeSelectorDialog(this, "n.a.");
        Dlg->setWindowTitle("discrete step Response");
    }
    else
    {
        Dlg = new RangeSelectorDialog(this, preExpression->getVariables());
        if(MenuEntry == QString("draw Response"))
        {
            Dlg->setWindowTitle("draw Response");
            Dlg->variableSelected("sel. Compl. e.g. s");
        }

        if(MenuEntry == "draw Bode")
        {
            Dlg->variableSelected("sel. Compl. e.g. s");
            Dlg->setWindowTitle("Bode Plot");
        }

        if(MenuEntry == "draw numeric Root Locus")
        {
            Dlg->variableSelected("sel. Gain");
            Dlg->setWindowTitle("root locus");
        }
    }
    delete preExpression;

RedoDlg:
    Dlg->setModal(true);
    if(!Dlg->exec())
        return;



    QPointF Range = Dlg->getRange();
    if(Range.x() >= Range.y())
    {
        QMessageBox Box;
        Box.setText(QString("Min/Max incorrect!!"));
        Box.setModal(true);
        Box.exec();
        goto RedoDlg;
    }

    QString VarName = Dlg->getVarName();
    if(VarName.count() == 0)
    {
        QMessageBox Box;
        Box.setText(QString("Variabel Name incorrect!!"));
        Box.setModal(true);
        Box.exec();
        goto RedoDlg;
    }


    if(VariabelMdl->getVarValuePtr(VarName) == NULL)
    {
        if(VarName != "n.a." && !VarName.contains("sel."))
            insertVariable(VarName+QString(" = 0.0"));
        if(VarName.contains("sel."))
            goto RedoDlg;
    }
    preExpression = ExpressionMdl->createExpression(Index, QString());

    QStringList VarStrings = preExpression->getVariables();
    QStringList MissingVars;
    for(int i = 0 ; i < VarStrings.count() ; i++)
    {
        if(NULL == VariabelMdl->getVarValuePtr(VarStrings.at(i)))
        {
            if(VarStrings.at(i) != "I" && VarStrings.at(i) != "i" &&
                    VarStrings.at(i) != "S" && VarStrings.at(i) != "s" &&
                    VarStrings.at(i) != "Z" && VarStrings.at(i) != "z"&&
                    VarStrings.at(i) != "x" && VarStrings.at(i) != "y")

                MissingVars.append(VarStrings.at(i));
        }
    }
    if(MissingVars.count())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Variables");
        msgBox.setText("Insert all missing Variables?");
        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::Yes)
        {

        }
        else
        {
            QString Message = "Variables Missing: ";
            for(int i = 0 ; i < MissingVars.count() ; i ++ )
            {
                Message += MissingVars.at(i);
                Message += " ";
                insertVariable(MissingVars.at(i)+QString(" = 1.0"));
            }

            QMessageBox Box;
            Box.setText(Message);
            Box.setModal(true);
            Box.exec();
            return;
        }

        for(int i = 0 ; i < MissingVars.count() ; i ++ )
        {
            insertVariable(MissingVars.at(i)+QString(" = 1.0"));
        }

    }


    double Resolution = Dlg->getResolution();
    QColor Color = Dlg->getColor();
    delete Dlg;
    if(MenuEntry == QString("draw Complex"))
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
            Box.setText(QString("Unnamed Functions are not allowed!!!\nPlease use 'Name:expr'."));
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
        enqueueCurve(Curve, ui->qwtPlot);
        Curve->setPen(QPen(Color));
        ui->qwtPlot->setAxisAutoScale( QwtPlot::xBottom );
        ui->qwtPlot->setAxisAutoScale( QwtPlot::yLeft );
        ui->qwtPlot->updateAxes();


        //Curve->setSymbol(QwtSymbol( QwtSymbol::Rect,
                      //                        QColor(Color), QColor(Color), QSize( 2, 2 ) ));
        IndependentMarkerSliderDialog->addSlider(VarName, Range);
        IndependentMarkerSliderDialog->show();
        IndependentMarkerSliderDialog->setVisible(true);
        IndependentMarkerSliderDialog->setFocus();
        IndependentMarkerSliderDialog->activateWindow();
        IndependentMarkerSliderDialog->raise();
    }


    if(MenuEntry == QString("draw Bode"))
    {
        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        if(FunctionName == QString())
        {
            QMessageBox Box;
            Box.setText(QString("Unnamed Functions are not allowed!!!\nPlease use 'Name:expr'."));
            Box.setModal(true);
            Box.exec();
            return;
        }
        BodeDialog *BodeDiagram;
        BodeDiagram = new BodeDialog(this);
        BodeDiagram->show();

        ControlExpression *MagExpression = ExpressionMdl->createExpression(Index, VarName);
        ControlExpression *PhaExpression = ExpressionMdl->createExpression(Index, VarName);
        EvalInfo Evinfo;
        Evinfo.IndepStart = Range.x();
        Evinfo.IndepEnd = Range.y();
        Evinfo.Resolution = Resolution;

        QwtMagnitudeCurve *MagCurve = new QwtMagnitudeCurve(MagExpression, Evinfo);
        QwtPhaseCurve *PhaCurve = new QwtPhaseCurve(PhaExpression, Evinfo);
        enqueueCurve(PhaCurve, BodeDiagram->getPhasePlot());
        enqueueCurve(MagCurve, BodeDiagram->getAmplitudePlot());

        connect(MagCurve, SIGNAL(amplitudeMarkerChangeSignal(double)), PhaCurve, SLOT(phaseMarkerChangeSlot(double)));
    }

    if(MenuEntry == QString("draw Response"))
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
        QwtResponseCurve *Curve;
        try
        {
            Curve = new QwtResponseCurve(Expression, Evinfo);
        }
        catch(const QString &e)
        {
            QMessageBox Box;
            Box.setText(e);
            Box.setModal(true);
            Box.exec();
            return;
        }

        enqueueCurve(Curve, StepRespDialog->getPlot());
        Curve->setPen(QPen(Color));

    }
    if(MenuEntry == QString("draw discrete step Response"))
    {
        StepResponseDialog *StepRespDialog = new StepResponseDialog(this);
        StepRespDialog->show();


        EvalInfo EvInfo;
        EvInfo.Dots = Range.y();
        QwtDiscreteResponseCurve *Curve = new QwtDiscreteResponseCurve(ExpressionMdl->getExpressionDefinition(Index),EvInfo);
        enqueueCurve(Curve, StepRespDialog->getPlot());
        Curve->setPen(QPen(Color));
    }
    if(MenuEntry == QString("draw numeric Root Locus"))
    {
        EvalInfo EvInfo;
        EvInfo.IndepStart = Range.x();
        EvInfo.IndepEnd = Range.y();
        EvInfo.Resolution = Resolution;
        QwtNumericRootLocusCurve *Curve;
        try
        {
            Curve = new QwtNumericRootLocusCurve(ExpressionMdl->createExpression(Index, VarName),EvInfo);
        }
        catch(const QString &e)
        {
            QMessageBox Box;
            Box.setText(e);
            Box.setModal(true);
            Box.exec();
            return;
        }


        enqueueCurve(Curve, ui->qwtPlot);
        Curve->setPen(QPen(Color));

        while(Curve->isRunning())
            usleep(100000);
        ui->qwtPlot->setAxisAutoScale( QwtPlot::xBottom );
        ui->qwtPlot->setAxisAutoScale( QwtPlot::yLeft );
        ui->qwtPlot->updateAxes();
        IndependentMarkerSliderDialog->addSlider(VarName, Range);
        IndependentMarkerSliderDialog->show();
        IndependentMarkerSliderDialog->setVisible(true);
        IndependentMarkerSliderDialog->setFocus();
        IndependentMarkerSliderDialog->activateWindow();
        IndependentMarkerSliderDialog->raise();
    }


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
    SliderMenu.addAction(QString("remove Slider"));
    SliderMenu.addAction(QString("delete Var"));
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
        insertSlider(VariableName, Range);
    }
    if(happened->text() == QString("remove Slider"))
    {
        VariableSliderDialog->removeSlider(VariableName);
    }

    if(happened->text() == QString("delete Var"))
    {
        VariableSliderDialog->removeSlider(VariableName);
        VariabelMdl->deleteVar(VariableName);
    }

}

void MainWindow::insertSlider(QString VariableName, QPointF Range)
{
    double *ActValue = VariabelMdl->getVarValuePtr(VariableName);

    VariableSliderDialog->addSlider(VariableName, Range, *ActValue);
    VariableSliderDialog->show();
    VariableSliderDialog->setVisible(true);
    VariableSliderDialog->setFocus();
    VariableSliderDialog->activateWindow();
    VariableSliderDialog->raise();
}


void MainWindow::emitAllValues(void)
{
    int numVariabels = VariabelMdl->rowCount();

    for(int i = 0 ; i < numVariabels ; i ++ )
    {
        QModelIndex Index = VariabelMdl->index(i);
        QString VarName = VariabelMdl->getVarName(Index);
        double *Val = VariabelMdl->getVarValuePtr(VarName);
        if(Val == NULL)
            return;
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

    }
    else
    {
        on_actionSave_as_triggered();
    }

    store(WorkFile, ExpressionMdl->getExpressionStringList());
    store(WorkFile+QString(".var"), VariabelMdl->getVarStringList());
    QMap <QString, QPointF> SliderMap;

    VariableSliderDialog->getSliders( &SliderMap);
    QStringList SliderList = SliderMap.keys();
    store(WorkFile+QString(".slder"), SliderList);
    QStringList RangeStrings;

    for(int i = 0 ; i < SliderList.count() ; i ++)
    {
        QPointF Range = SliderMap.value(SliderList.at(i));
        QString RangeString;
        RangeString.sprintf("%f %f", Range.x(), Range.y());
        RangeStrings.append(RangeString);
    }
    store(WorkFile+QString(".sldra"), RangeStrings);

    storeFile(WorkFile+QString(".doc"), ControlSystemDocDlg->getText());
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

            QStringList SliderVariables = load(dlg.selectedFiles().at(0)+QString(".slder"));
            QStringList SliderRanges = load(dlg.selectedFiles().at(0)+QString(".sldra"));
            for(int i = 0 ; i < SliderVariables.count() ; i ++ )
            {
                QPointF Range;
                Range.setX(SliderRanges.at(i).toDouble());
                int idx = SliderRanges.at(i).indexOf(" ");
                QString YString = SliderRanges.at(i).right(SliderRanges.at(i).length()-idx);
                Range.setY(YString.toDouble());
                insertSlider(SliderVariables.at(i), Range);
            }
            ControlSystemDocDlg->setText(loadFile(dlg.selectedFiles().at(0)+QString(".doc")));
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
            on_actionSave_triggered();
        }
    }
}


void MainWindow::deleteAllCurves(void)
{
    for(; CurveMdl->rowCount() ; )
    {
        QwtControlPlotItem *CurveItem = CurveMdl->at(0);
        CurveMdl->removeCurve(CurveItem);
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
    QwtControlPlotItem *CurveItem = CurveMdl->at(index.row());

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("delete"));
    ExpressionMenu.addAction(QString("toggle visability"));
    ExpressionMenu.addAction(QString("setup"));
    ExpressionMenu.addAction(QString("install Tracker"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    if(happened->text() == QString("delete"))
    {
        CurveMdl->removeCurve(CurveItem);
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

    if(happened->text() == QString("setup"))
    {
        QwtControlPlotItem *Item = (QwtControlPlotItem *)CurveItem;
        Item->setup();
    }
    if(happened->text() == QString("install Tracker"))
    {
        ControlSystemTracker *Tracker = new ControlSystemTracker(this);
        connect(CurveItem, SIGNAL(propertyChange(double, QString)), Tracker, SLOT(SystemProperty(double,QString)));
        Tracker->show();
    }
}

void MainWindow::on_CurveListView_doubleClicked(const QModelIndex &index)
{
    QwtControlPlotItem *Curve = CurveMdl->at(index.row());
    EvalInfo *EvInfo;
    ControlExpression *pExpression;
    pExpression = Curve->getExpressionPtr();

    if(pExpression == NULL)
        return;

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
    QwtPlot *Plot =  CurveMdl->at(index.row())->plot();
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
    CurveMdl->appendCurve((QwtControlPlotItem*)Curve);
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

QString MainWindow::loadFile(QString FilePath)
{
    QString Text;



    QFile fIn(FilePath);
    if (fIn.open(QFile::ReadOnly | QFile::Text))
    {
        Text= QString(fIn.read(1024*1024*10));
    }
    else
    {
        std::cerr << "error opening output file\n";
        return QString();
    }

    return Text;
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

bool MainWindow::storeFile(QString FilePath, QString Text)
{
    // write data
    QFile fOut(FilePath);

    if (fOut.open(QFile::WriteOnly | QFile::Text))
    {
        fOut.write(Text.toStdString().c_str());
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
    d_zoomer->setZoomBase( true );
    connect(d_zoomer, SIGNAL(selected(QwtDoubleRect)), this, SLOT(rlZoomerSelected(QwtDoubleRect)));


    ui->qwtPlot->updateAxes();
    ui->qwtPlot->replot();
}

void MainWindow::on_actionSystem_triggered()
{
    QString Expression;
    SystemDialog Dlg((QWidget*)this, &Expression);
    Dlg.setModal(true);
    Dlg.exec();
}

void MainWindow::on_actionView_Control_System_Doc_triggered()
{
    ControlSystemDocDlg->show();
}

void MainWindow::on_actionHelp_triggered()
{
    QDir HelpDir("Doc");
    QStringList Filters;
    Filters.append("*.pdf");
    QStringList PdfEntries = HelpDir.entryList(Filters);
    HelpSelectorDialog Dlg(this, PdfEntries);
    Dlg.setModal(true);

    if(Dlg.exec())
    {
        QString CMD;
        CMD.sprintf("okular Doc/%s &", Dlg.getSelectedHelpEntry().toStdString().c_str());
        system(CMD.toStdString().c_str());
    }
}

void MainWindow::on_actionDiscrete_System_triggered()
{
    QString Expression;
    DiscreteSystemDialog Dlg((QWidget*)this, &Expression);
    Dlg.setModal(true);
    Dlg.exec();
}

void MainWindow::on_actionDiscrete_Continous_System_triggered()
{
redoDCS:
    DiscreteContinousSystemDialog Dlg(this);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;


    QStringList Expressions = Dlg.getExpressions();

    if(Expressions.at(0).count() == 0 ||
            Expressions.at(1).count() == 0)
    {
        QMessageBox Box;
        Box.setText("Please give proper Expressions!");
        Box.setModal(true);
        Box.exec();
        goto redoDCS;
    }


    Expressions.replace(0, inverseZTransform(Expressions.at(0)));
    EvalInfo EvInfo = Dlg.getEvalInfo();
    QwtDiscreteContinousResponseCurve *Curve = new QwtDiscreteContinousResponseCurve(Expressions, EvInfo);


    StepResponseDialog *StepRespDialog = new StepResponseDialog(this);
    StepRespDialog->show();
    enqueueCurve(Curve, StepRespDialog->getPlot());
    Curve->setPen(QPen("red"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox Box;
    Box.setText("Laplace Explorer(C) 2013\nFlorian Hillen");
    Box.setModal(true);
    Box.exec();
}

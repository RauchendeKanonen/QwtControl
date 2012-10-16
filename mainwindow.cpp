#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModelIndex>
#include "expressionmodel.h"
#include "varmodel.h"
#include "numericallaplace.h"
#include "mathFunction/mathfunctioncompiler.h"
#include "mathFunction/mathfunctionevaluator.h"
#include "mathFunction/mathfunctionpreprocessor.h"
#include "controlexpression.h"


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
    CurveMdl->setCurveList(&CurveInformationList);

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


}

MainWindow::~MainWindow()
{
    delete ui;
    if(VariableSliderDialog)
        delete VariableSliderDialog;
    if(IndependentMarkerSliderDialog)
        delete IndependentMarkerSliderDialog;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    event = event;
    delete VariableSliderDialog;
    VariableSliderDialog = NULL;
    delete IndependentMarkerSliderDialog;
    IndependentMarkerSliderDialog = NULL;
}


void MainWindow::on_actionExpression_triggered()
{
    QString Expression;
    ExpressionDialog Dlg(this, &Expression);
    Dlg.setModal(true);
    Dlg.exec();

    QModelIndex index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->setData(index, Expression,Qt::EditRole);
}



void MainWindow::on_actionVariable_triggered()
{
    Value Val;
    QString Name;
    VarDialog Dlg(this, &Val, &Name);
    Dlg.setModal(true);
    Dlg.exec();

    if(!Dlg.exec())
        return;

    QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->setData(index, QString(Name + QString(" = ") + QString(Val.ToString().c_str())),Qt::EditRole);
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

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

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

    if(happened->text() == QString("draw Root Locus"))
    {
        ParserX *Expression = ExpressionMdl->getExpression(Index);
        ParserX *BaseExpression = ExpressionMdl->getExpression(Index);
        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        if(false == updateExpressionVars(Expression, VarName))
            return;


        //add the marker to the variables
        if(NULL == VariabelMdl->getVarValuePtr(VarName))
        {
            Value Val;
            QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
            index = VariabelMdl->index(0, 0, QModelIndex());
            VariabelMdl->setData(index, QString(VarName + QString(" = ") + QString(Val.ToString().c_str())),Qt::EditRole);
        }



        IndependentMarkerSliderDialog->addSlider(VarName, Range);
        IndependentMarkerSliderDialog->show();
        IndependentMarkerSliderDialog->setVisible(true);
        IndependentMarkerSliderDialog->setFocus();
        IndependentMarkerSliderDialog->activateWindow();
        IndependentMarkerSliderDialog->raise();

        QwtPlotControlCurve* Curve = new QwtPlotControlCurve();
        initCurveInformationStruct(ui->qwtPlot, Expression, BaseExpression,
                                   Range.x(), Range.y(), Resolution, CURVE_TYPE_COMPLEX,
                                   VarName, Curve, NULL, NULL, Color, FunctionName);
    }


    if(happened->text() == QString("draw Bode"))
    {
        BodeDialog *BodeDiagram;
        BodeDiagram = new BodeDialog(this);
        BodeDiagram->show();



       /* QListWidgetItem *Item = ui->ComplexExpressionListWidget->item(t.row());
        int Index = Item->data(Qt::UserRole).toInt();
        ParserX *Expression = ExpressionList.at(Index);

        BodeDiagram->setExpression(Expression);
        if(false == updateExpressionVars(Expression))
            return;

        BodeDiagram->replot();
        BodeList.append(BodeDiagram);*/
    }

    if(happened->text() == QString("draw Response"))
    {
        StepResponseDialog *StDialog = new StepResponseDialog(this);
        StDialog->show();

        ParserX *Expression = ExpressionMdl->getExpression(Index);
        ParserX *BaseExpression = ExpressionMdl->getExpression(Index);

        QString FunctionName = ExpressionMdl->getExpressionName(Index);
        if(FunctionName == QString())
        {
            QMessageBox Box;
            Box.setText(QString("Unnamed Functions are not allowed!!!"));
            Box.setModal(true);
            Box.exec();
            delete StDialog;
            return;
        }

        QString Function = QString::fromStdString(Expression->GetExpr());

        mathFunctionpreprocessor Processor(Function);
        bool procState = Processor.getState();
        if(!procState)
        {
            QMessageBox Box;
            Box.setText(QString("Error while preprocessing Function!!!"));
            Box.setModal(true);
            Box.exec();
            delete StDialog;
            return;
        }



        Function = Processor.preprocessedString();
        mathFunctionCompiler Compiler(Function, VarName, FunctionName);
        bool comState = Compiler.getState();
        if(!comState)
        {
            QMessageBox Box;
            Box.setText(QString("Error while compiling/linking Function!!!"));
            Box.setModal(true);
            Box.exec();
            delete StDialog;
            return;
        }


        mathFunctionEvaluator *Evaluator = new mathFunctionEvaluator(VarName, FunctionName);
        bool evalState = Evaluator->getState();
        if(!evalState)
        {
            QMessageBox Box;
            Box.setText(QString("Error while evaluating Function!!!"));
            Box.setModal(true);
            Box.exec();
            delete StDialog;
            return;
        }


        if(false == updateExpressionVars(Expression, VarName))
        {
            QMessageBox Box;
            Box.setText(QString("Error undefined Variables!!!"));
            Box.setModal(true);
            Box.exec();
            delete StDialog;
            return;
            delete StDialog;
            return;
        }

        initCurveInformationStruct(StDialog->getPlot(), Expression, BaseExpression,
                                   Range.x(), Range.y(), Resolution, CURVE_TYPE_XY_COMPILED_NUMERICAL,
                                   VarName, StDialog->getPlotCurve(), Evaluator, VariabelMdl, Color, FunctionName);
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

        double ActValue = VariabelMdl->getVarValuePtr(VariableName)->GetFloat();

        VariableSliderDialog->addSlider(VariableName, Range, ActValue);
        VariableSliderDialog->show();
        VariableSliderDialog->setVisible(true);
        VariableSliderDialog->setFocus();
        VariableSliderDialog->activateWindow();
        VariableSliderDialog->raise();
    }
}

bool MainWindow::updateExpressionVars(ParserX *Expression, QString IndependentVar)
{
    try
    {
        Expression->ClearVar();
        var_maptype vmap = Expression->GetExprVar();
        for (var_maptype::iterator item = vmap.begin(); item!=vmap.end(); ++item)
        {

            if(QString::fromStdString(item->first) == IndependentVar)
                            continue;

            Value *Val;
            if((Val = VariabelMdl->getVarValuePtr(QString::fromStdString(item->first))) != NULL)
            {
                Expression->DefineVar(item->first, Val);
            }

            else if(QString::fromStdString(item->first) != IndependentVar)
                return false;

            cout << "  " << item->first << " =  " << (Variable&)(*(item->second)) << "\n";
        }
    }
    catch(mup::ParserError &e)
    {
        return false;
    }
    return true;
}

void MainWindow::initCurveInformationStruct(QwtPlot *Plot, ParserX *Expression, ParserX *BaseExpression,
                                            double StartPoint, double EndPoint, double Resolution, int CurveType,
                                            QString IndependentVar, QwtPlotControlCurve *Curve, mathFunctionEvaluator *Evaluator,
                                            VarModel *pVariabelMdl, QColor Color, QString FuctionName)
{

    CurveInformationStruct *CurveItem = new CurveInformationStruct;
    CurveItem->Sem = new QSemaphore();
    CurveItem->Sem->release();
    CurveItem->Curve = Curve;

    CurveItem->CurveType = CurveType;

    CurveItem->pVariabelMdl = pVariabelMdl;
    CurveItem->Evaluator = Evaluator;


    CurveItem->Marker = new QwtPlotMarker();


    CurveItem->MarkerPos = 0.0;
    CurveItem->Color = Color;
    if(CurveType == CURVE_TYPE_COMPLEX)
    {
        CurveItem->PoleLocation = new QwtPlotMarker();

        CurveItem->RootLocation = new QwtPlotMarker();

        CurveItem->PoleLocation->setSymbol( QwtSymbol( QwtSymbol::XCross,
                                                         QColor(Color), QColor(Color), QSize( 15, 15 ) ) );

        CurveItem->RootLocation->setSymbol( QwtSymbol( QwtSymbol::Ellipse,
                                                         QColor(Color), QColor(Color), QSize( 10, 10 ) ) );

        CurveItem->Curve->setPen(QPen(Color));
        CurveItem->Curve->setSymbol(QwtSymbol( QwtSymbol::Rect,
                                              QColor(Color), QColor(Color), QSize( 2, 2 ) ));
        CurveItem->Marker->setSymbol( QwtSymbol( QwtSymbol::Cross,
                                                 QColor(Color), QColor(Color), QSize( 25, 25 ) ) );
        CurveItem->Marker->attach(Plot);

        CurveItem->RootLocation->attach(Plot);
        CurveItem->PoleLocation->attach(Plot);
    }

    memset(CurveItem->IndepVarName, 0, sizeof(CurveItem->IndepVarName));
    memcpy(CurveItem->IndepVarName, IndependentVar.toStdString().c_str(), strlen(IndependentVar.toStdString().c_str()));

    CurveItem->Resolution = Resolution;
    CurveItem->StartPoint = StartPoint;
    CurveItem->EndPoint = EndPoint;
    CurveItem->DataSize = (EndPoint - StartPoint)/Resolution;

    CurveItem->xData = (double*)malloc( CurveItem->DataSize * sizeof(double));
    CurveItem->yData = (double*)malloc( CurveItem->DataSize * sizeof(double));
    CurveItem->Expression = Expression;
    CurveItem->BaseExpression = BaseExpression;
    CurveItem->Plot = Plot;
    CurveItem->Curve->attach(Plot);

    CurveItem->Thread = (CurveThread*)new CurveThread(NULL, CurveItem);
    connect(CurveItem->Thread, SIGNAL(CurveReady(CurveInformationStruct *)), this, SLOT( CurveReady(CurveInformationStruct *)));


    CurveInformationList.append(CurveItem);
    ((CurveThread*)CurveItem->Thread)->start();


}

QList <CurveInformationStruct*> *MainWindow::dependingCurves(QString VarName)
{
    QList <CurveInformationStruct*> *DependingList = new QList <CurveInformationStruct*>();
    try
    {
        for(int i = 0 ; i < CurveInformationList.count(); i++)
        {
            ParserX *Expression = CurveInformationList.at(i)->BaseExpression;
            Expression->ClearVar();
            var_maptype vmap = Expression->GetExprVar();
            for (var_maptype::iterator item = vmap.begin(); item!=vmap.end(); ++item)
            {
                QString Test = QString::fromStdString(item->first);
                if(QString::fromStdString(item->first) == VarName)
                {
                    DependingList->append(CurveInformationList.at(i));
                }
            }
            updateExpressionVars(Expression, CurveInformationList.at(i)->IndepVarName);
        }
    }
    catch(mup::ParserError &e)
    {
        delete DependingList;
        return NULL;
    }
    return DependingList;
}

void MainWindow::CurveReady(CurveInformationStruct *CurveInfo)
{
    CurveInfo->Curve->setData(CurveInfo->xData, CurveInfo->yData, CurveInfo->DataSize);
    CurveInfo->Plot->replot();
}

void MainWindow::lockAll(void)
{
    for(int i = 0 ; i < CurveInformationList.count() ; i ++)
    {
        CurveInformationList.at(i)->Sem->acquire();
    }

}

void MainWindow::releaseAll(void)
{
    for(int i = 0 ; i < CurveInformationList.count() ; i ++)
    {
        CurveInformationList.at(i)->Sem->release();
    }

}

bool MainWindow::waitAll(void)
{
    for(int i = 0 ; i < CurveInformationList.count() ; i ++)
    {

        if(!CurveInformationList.at(i)->Thread->isFinished())
            return false;
    }
    return true;
}

void MainWindow::parameterChange(QString VarName, double DblVal)
{
    Value *Val = VariabelMdl->getVarValuePtr(VarName);

    if(!waitAll())
        return;

    if(Val != NULL)
    {
        *Val = DblVal;

        QList <CurveInformationStruct*> *DependingList = dependingCurves(VarName);

        if(DependingList == NULL)
            return;

        for(int i = 0 ; i < DependingList->count() ; i ++)
        {
            ((CurveThread*)DependingList->at(i)->Thread)->start();
        }
        VariabelMdl->valueChange();
    }

}

void MainWindow::markerChange(QString VarName, double DblVal)
{
    Value *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        *Val = DblVal;
        VariabelMdl->valueChange();
    }


    QList <CurveInformationStruct*> *DependingList = dependingCurves(VarName);
    if(DependingList == NULL)
        return;
    for(int i = 0 ; i < DependingList->count() ; i ++)
    {
        CurveInformationStruct *CurveInfo = DependingList->at(i);

        CurveInfo->MarkerPos = DblVal;
        if(CurveInfo->CurveType == CURVE_TYPE_COMPLEX)
        {
            Value InVal = CurveInfo->MarkerPos;
            Variable Var(&InVal);
            CurveInfo->Expression->DefineVar(VarName.toStdString(), &Var);
            Value MarkerResult = CurveInfo->Expression->Eval();
            CurveInfo->Marker->setYValue(MarkerResult.GetImag());
            CurveInfo->Marker->setXValue(MarkerResult.GetFloat());
            CurveInfo->Plot->replot();
            continue;
        }
        if(((CurveThread*)DependingList->at(i)->Thread)->isFinished())
            ((CurveThread*)DependingList->at(i)->Thread)->start();

    }
    return;
}

void MainWindow::on_ExpressionListView_doubleClicked(const QModelIndex &index)
{
    QString Expression = ExpressionMdl->getExpressionDefinition(index);
    ExpressionDialog Dlg(this, &Expression);
    Dlg.setModal(true);
    Dlg.exec();

    ExpressionMdl->setData(index, QString(Expression),Qt::EditRole);
}

void MainWindow::on_VariableListView_doubleClicked(const QModelIndex &index)
{

    QString Name = VariabelMdl->getVarName(index);
    Value *Val = VariabelMdl->getVarValuePtr(Name);

    VarDialog Dlg(this, Val, &Name);
    Dlg.setModal(true);
    Dlg.exec();


    VariabelMdl->setData(index, QString(Name + QString(" = ") + QString(Val->ToString().c_str())),Qt::EditRole);
}



void MainWindow::load(QString FilePath)
{

    int INFILE;

    QString FileName = FilePath;
#ifdef LINUX
    if((INFILE = ::open(FileName.toStdString().c_str(),O_RDONLY))<=0)
    {
        QMessageBox Box;
        Box.setText(QString("KannConfiguration nicht finden!!!!"));
        Box.setModal(true);
        Box.exec();
    }

#endif
#ifdef WINDOWS
    if((INFILE = ::open((QString(RELDATAPATH)+QString("/config")).toStdString().c_str(), O_RDONLY | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
    {
        int i = GetLastError();
        QMessageBox Box;
        Box.setText(QString("KannConfiguration nicht finden!!!!"));
        Box.setModal(true);
        Box.exec();
    }
#endif
    char ExpressionBuffer[1024*1024];
    char *ptr = ExpressionBuffer;
    int size;

    if((size = ::read(INFILE, (char*)ExpressionBuffer, 1024*1024)) <= 0)
    {

    }
    else
    {
        while((long)ptr < ((long)ExpressionBuffer+(long)size))
        {
            QString Expression(ptr);
            ExpressionMdl->insertRows(0,1);
            ExpressionMdl->setData(ExpressionMdl->index(0), QVariant(Expression), Qt::EditRole);
            ptr += strlen(ptr)+1;
        }

    }
    ::close(INFILE);

    FileName += QString(".var");
#ifdef LINUX
    if((INFILE = ::open(FileName.toStdString().c_str(),O_RDONLY))<=0)
    {
        QMessageBox Box;
        Box.setText(QString("KannConfiguration nicht finden!!!!"));
        Box.setModal(true);
        Box.exec();
    }

#endif
#ifdef WINDOWS
    if((INFILE = ::open((QString(RELDATAPATH)+QString("/config")).toStdString().c_str(), O_RDONLY | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
    {
        int i = GetLastError();
        QMessageBox Box;
        Box.setText(QString("KannConfiguration nicht finden!!!!"));
        Box.setModal(true);
        Box.exec();
    }
#endif

    char DeclarationBuffer[1024*1024];
    char *DeclarationPtr = DeclarationBuffer;

    if((size = ::read(INFILE, (char*)DeclarationBuffer, 1024*1024)) <= 0)
    {

    }
    else
    {
        while((long)DeclarationPtr < ((long)DeclarationBuffer+(long)size))
        {
            QString DeclarationString(DeclarationPtr);
            int Index = DeclarationString.indexOf(QString(" "), 0, Qt::CaseSensitive);
            QString DecName = DeclarationString.left(Index);
            Index = DeclarationString.indexOf(QString(" "), Index+1, Qt::CaseSensitive);
            QString ValueString = DeclarationString.right(DeclarationString.length()-Index-1);

            VariabelMdl->insertRows(0,1);
            VariabelMdl->setData(VariabelMdl->index(0), QVariant(DecName + QString(" = ") + ValueString), Qt::EditRole);
            DeclarationPtr += strlen(DeclarationPtr)+1;
        }
        VariabelMdl->valueChange();
    }
    ::close(INFILE);

    return;
}

void MainWindow::store(QString FilePath)
{
    char ExpressionBuffer[1024*1024];
    char *ptr=ExpressionBuffer;
    for(int i = ExpressionMdl->rowCount() -1  ; i >= 0  ; i -- )
    {
        const char *Expression = ExpressionMdl->getExpressionDefinition(ExpressionMdl->index(i)).toStdString().c_str();
        memcpy((void*)ptr, Expression, strlen(Expression)+1);
        ptr += (long)strlen(Expression);
        *ptr = 0;
        ptr += 1;
    }

    int OUTFILE;
#ifdef LINUX
    if((OUTFILE = ::open(FilePath.toStdString().c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
        return ;
#endif

#ifdef WINDOWS
    QFile OldOne(Filename);
    OldOne.remove();
    if((OUTFILE = ::open(FilePath.c_str(),O_CREAT | O_WRONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
        return ;
#endif


    ::write(OUTFILE, (char*)ExpressionBuffer, (long)ptr-(long)ExpressionBuffer);
    ::close(OUTFILE);


    FilePath += QString(".var");

    char DeclarationBuffer[1024*1024];
    char *DeclarationPtr=DeclarationBuffer;

    for(int i = VariabelMdl->rowCount()-1 ; i >= 0  ; i --)
    {
        QString Declaration = QString(VariabelMdl->data(VariabelMdl->index(i), Qt::DisplayRole).toString());
        const char *DeclarationStr = Declaration.toStdString().c_str();
        memcpy(DeclarationPtr, DeclarationStr, strlen(DeclarationStr)+1);
        DeclarationPtr += strlen(DeclarationStr)+1;
    }

#ifdef LINUX
    if((OUTFILE = ::open(FilePath.toStdString().c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
        return ;
#endif

#ifdef WINDOWS
    QFile OldOne(Filename);
    OldOne.remove();
    if((OUTFILE = ::open(FilePath.c_str(),O_CREAT | O_WRONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
        return ;
#endif


    ::write(OUTFILE, (char*)DeclarationBuffer, (long)DeclarationPtr-(long)DeclarationBuffer);
    ::close(OUTFILE);



    return;
}


void MainWindow::on_actionSave_triggered()
{
    if(QString("") != WorkFile)
    {
        store(WorkFile);
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

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            setWindowTitle(dlg.selectedFiles().at(0));
            load(dlg.selectedFiles().at(0));
            WorkFile = dlg.selectedFiles().at(0);
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
            setWindowTitle(dlg.selectedFiles().at(0));

            QString Filename = dlg.selectedFiles().at(0);
            setWindowTitle(dlg.selectedFiles().at(0));
            store(Filename);
            WorkFile = Filename;
        }
    }
}

void MainWindow::on_CurveListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->CurveListView->mapToGlobal(pos); // Map the global position to the userlist
    QModelIndex index = ui->CurveListView->indexAt(pos);
    if(!index.isValid())
        return;
    CurveInformationStruct *CurveInfo = CurveInformationList.at(index.row());

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("delete"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    if(happened->text() == QString("delete"))
    {
        deleteCurve(CurveInfo);
    }

}

void MainWindow::deleteCurve(CurveInformationStruct *CurveInfo)
{

    if(CurveInfo->CurveType == CURVE_TYPE_XY_COMPILED_NUMERICAL)
    {
        CurveInformationList.removeOne(CurveInfo);

        CurveInfo->Thread->terminate();
        delete CurveInfo->Thread;
        CurveInfo->Curve->detach();
        delete CurveInfo->Curve;
        CurveInfo->Marker->detach();
        delete CurveInfo->Marker;
        delete CurveInfo->Evaluator;
        QWidget *Wdg = (QWidget*) CurveInfo->Plot->parent();
        delete CurveInfo->Plot;
        delete Wdg;
        free(CurveInfo->xData);
        free(CurveInfo->yData);
    }
    else if(CurveInfo->CurveType == CURVE_TYPE_COMPLEX)
    {
        CurveInformationList.removeOne(CurveInfo);
        CurveInfo->Thread->terminate();
        delete CurveInfo->Thread;
        CurveInfo->Curve->detach();
        delete CurveInfo->Curve;
        CurveInfo->Marker->detach();
        delete CurveInfo->Marker;
        CurveInfo->RootLocation->detach();
        delete CurveInfo->RootLocation;
        CurveInfo->PoleLocation->detach();
        delete CurveInfo->PoleLocation;
        CurveInfo->Plot->replot();
        free(CurveInfo->xData);
        free(CurveInfo->yData);
    }
}

void MainWindow::on_CurveListView_doubleClicked(const QModelIndex &index)
{
    CurveInformationStruct *CurveInfo = CurveInformationList.at(index.row());
    RangeSelectorDialog Dlg(this, QString(CurveInfo->IndepVarName));
    Dlg.setRange(QPointF(CurveInfo->StartPoint, CurveInfo->EndPoint));
    Dlg.setResolution(CurveInfo->Resolution);
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;


    QPointF Range = Dlg.getRange();
    double Resolution = Dlg.getResolution();


    CurveInfo->StartPoint = Range.x();
    CurveInfo->EndPoint = Range.y();
    CurveInfo->Resolution = Resolution;
    markerChange(CurveInfo->IndepVarName, CurveInfo->MarkerPos);

    IndependentMarkerSliderDialog->addSlider(CurveInfo->IndepVarName, Range);
    IndependentMarkerSliderDialog->show();
    IndependentMarkerSliderDialog->setVisible(true);
    IndependentMarkerSliderDialog->setFocus();
    IndependentMarkerSliderDialog->activateWindow();
    IndependentMarkerSliderDialog->raise();

}

void MainWindow::on_CurveListView_clicked(const QModelIndex &index)
{
    QWidget *Wdg = (QWidget*) CurveInformationList.at(index.row())->Plot->parent();
    Wdg->setVisible(true);
    Wdg->setFocus();
    Wdg->activateWindow();
    Wdg->raise();

}



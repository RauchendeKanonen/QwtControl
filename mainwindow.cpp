#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QModelIndex>
#include "expressionmodel.h"
#include "varmodel.h"
#include "numericallaplace.h"


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

    ui->ExpressionListView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->VariableListView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->CurveListView->setContextMenuPolicy(Qt::CustomContextMenu);

    VariableSliderDialog = new ParameterSliderDialog();
    connect(VariableSliderDialog, SIGNAL(parameterChange(QString , double )), this, SLOT(parameterChange(QString , double )));
    IndependentMarkerSliderDialog = new ParameterSliderDialog();
    connect(IndependentMarkerSliderDialog, SIGNAL(parameterChange(QString , double )), this, SLOT(markerChange(QString , double )));


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
    ParserX *pParser = new ParserX();
    ExpressionDialog Dlg(this, pParser);
    Dlg.setModal(true);
    Dlg.exec();

    QModelIndex index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = ExpressionMdl->index(0, 0, QModelIndex());
    ExpressionMdl->setData(index, QString(pParser->GetExpr().c_str()),Qt::EditRole);
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
    ExpressionMenu.addAction(QString("draw as Complex Funktion"));
    ExpressionMenu.addAction(QString("draw Bode"));
    ExpressionMenu.addAction(QString("draw Step Response"));
    ExpressionMenu.addAction(QString("draw Step Response from Numerical Solution"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    RangeSelectorDialog Dlg(this, QString());
    Dlg.setModal(true);
    if(!Dlg.exec())
        return;


    QPointF Range = Dlg.getRange();
    QString VarName = Dlg.getVarName();
    double Resolution = Dlg.getResolution();

    if(happened->text() == QString("draw as Complex Funktion"))
    {
        ParserX *Expression = ExpressionMdl->getExpression(Index);
        ParserX *BaseExpression = ExpressionMdl->getExpression(Index);
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


        initCurveInformationStruct(ui->qwtPlot, Expression, BaseExpression,
                                   Range.x(), Range.y(), Resolution, CURVE_TYPE_COMPLEX,
                                   VarName, new QwtPlotCurve());
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

    if(happened->text() == QString("draw Step Response"))
    {
        StepResponseDialog *StDialog = new StepResponseDialog(this);
        StDialog->show();

        ParserX *Expression = ExpressionMdl->getExpression(Index);
        ParserX *BaseExpression = ExpressionMdl->getExpression(Index);
        if(false == updateExpressionVars(Expression, VarName))
        {
            delete StDialog;
            return;
        }

        initCurveInformationStruct(StDialog->getPlot(), Expression, BaseExpression,
                                   Range.x(), Range.y(), Resolution, CURVE_TYPE_XY,
                                   VarName, StDialog->getPlotCurve());
    }
    if(happened->text() == QString("draw Step Response from Numerical Solution"))
    {

        StepResponseDialog *StDialog = new StepResponseDialog(this);
        StDialog->show();

        ParserX *Expression = ExpressionMdl->getExpression(Index);
        ParserX *BaseExpression = ExpressionMdl->getExpression(Index);
        if(false == updateExpressionVars(Expression, VarName))
        {
            delete StDialog;
            return;
        }

        initCurveInformationStruct(StDialog->getPlot(), Expression, BaseExpression,
                                   Range.x(), Range.y(), Resolution, CURVE_TYPE_XY_NUMERICAL,
                                   VarName, StDialog->getPlotCurve());
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


        VariableSliderDialog->addSlider(VariableName, Range);
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
                                            QString IndependentVar, QwtPlotCurve *Curve)
{

    if(CurveType != CURVE_TYPE_XY_NUMERICAL)
    {
        CurveInformationStruct *CurveItem = new CurveInformationStruct;
        CurveItem->Sem = new QSemaphore();
        CurveItem->Sem->release();
        CurveItem->Curve = Curve;
        CurveItem->CurveType = CurveType;

        CurveItem->Marker = new QwtPlotMarker();
        CurveItem->MarkerPos = 0.0;

        if(CurveType == CURVE_TYPE_COMPLEX)
        {

            CurveItem->Marker->setSymbol( QwtSymbol( QwtSymbol::Diamond,
                                                     QColor( Qt::yellow ), QColor( Qt::green ), QSize( 7, 7 ) ) );
            CurveItem->Marker->attach(Plot);
        }

        memset(CurveItem->IndepVarName, 0, sizeof(CurveItem->IndepVarName));
        memcpy(CurveItem->IndepVarName, IndependentVar.toStdString().c_str(), strlen(IndependentVar.toStdString().c_str()));

        CurveItem->Segment = NO_SUBSEGMENTS;
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
    else
    {
        CurveInformationStruct *CurveItem = new CurveInformationStruct;
        CurveItem->Resolution = Resolution;
        CurveItem->DataSize = ((EndPoint - StartPoint)/Resolution);
        CurveItem->Curve = Curve;
        CurveItem->CurveType = CurveType;
        CurveItem->Plot = Plot;
        CurveItem->BaseExpression = BaseExpression;
        CurveItem->SubSegmentInformation = (CurveInformationStruct**)malloc(sizeof(CurveInformationStruct*)*NumThreads);
        CurveItem->Segment = N_SUBSEGMENTS;
        CurveItem->xData = (double*)malloc( CurveItem->DataSize * sizeof(double));
        CurveItem->yData = (double*)malloc( CurveItem->DataSize * sizeof(double));
        CurveItem->StartPoint = StartPoint;
        CurveItem->EndPoint = EndPoint;


        for(int i = 0 ; i < NumThreads ; i ++ )
        {
            CurveInformationStruct *SubCurveItem = new CurveInformationStruct;
            CurveItem->SubSegmentInformation[i] = SubCurveItem;
            SubCurveItem->ParentInformation = CurveItem;
            SubCurveItem->Sem = new QSemaphore();
            SubCurveItem->Sem->release();
            SubCurveItem->Curve = Curve;
            SubCurveItem->CurveType = CurveType;

            SubCurveItem->MarkerPos = 0.0;

            memset(SubCurveItem->IndepVarName, 0, sizeof(SubCurveItem->IndepVarName));
            memcpy(SubCurveItem->IndepVarName, IndependentVar.toStdString().c_str(), strlen(IndependentVar.toStdString().c_str()));


            SubCurveItem->Resolution = Resolution;
            SubCurveItem->DataSize = ((EndPoint - StartPoint)/Resolution) / NumThreads;
            SubCurveItem->xData = (double*)malloc( SubCurveItem->DataSize * sizeof(double));
            SubCurveItem->yData = (double*)malloc( SubCurveItem->DataSize * sizeof(double));

            SubCurveItem->StartPoint = StartPoint + i * (Resolution*(SubCurveItem->DataSize));
            SubCurveItem->EndPoint = SubCurveItem->StartPoint + (Resolution*(SubCurveItem->DataSize));



            QString ExpressionString = QString(Expression->GetExpr().c_str());

            SubCurveItem->Expression = new ParserX();
            SubCurveItem->Expression->SetExpr(ExpressionString.toStdString().c_str());
            if(!updateExpressionVars(SubCurveItem->Expression, SubCurveItem->IndepVarName))
                return;

            SubCurveItem->Segment = i;
            SubCurveItem->BaseExpression = BaseExpression;
            SubCurveItem->Thread = (CurveSegmentThread*)new CurveSegmentThread(NULL, SubCurveItem);
            SubCurveItem->Plot = Plot;
            SubCurveItem->Curve->attach(Plot);

            connect(SubCurveItem->Thread, SIGNAL(CurveSegmentReady(CurveInformationStruct *)), this, SLOT( CurveSegmentReady(CurveInformationStruct *)));
            ((CurveSegmentThread*)SubCurveItem->Thread)->start();
        }
        CurveInformationList.append(CurveItem);
    }
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


void MainWindow::CurveSegmentReady(CurveInformationStruct *CurveInfo)
{
    CurveInformationStruct *Parent = CurveInfo->ParentInformation;

    memcpy(Parent->xData+(CurveInfo->DataSize)*CurveInfo->Segment, CurveInfo->xData, CurveInfo->DataSize*sizeof(double));
    memcpy(Parent->yData+(CurveInfo->DataSize)*CurveInfo->Segment, CurveInfo->yData, CurveInfo->DataSize*sizeof(double));

    CurveInfo->Curve->setData(Parent->xData, Parent->yData, Parent->DataSize);
    CurveInfo->Plot->replot();

}

void MainWindow::CurveReady(CurveInformationStruct *CurveInfo)
{
    CurveInfo->Curve->setData(CurveInfo->xData, CurveInfo->yData, CurveInfo->DataSize);
    CurveInfo->Plot->replot();
}



void MainWindow::parameterChange(QString VarName, double DblVal)
{
    Value *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        *Val = DblVal;

        QList <CurveInformationStruct*> *DependingList = dependingCurves(VarName);

        for(int i = 0 ; i < DependingList->count() ; i ++)
        {
            if(DependingList->at(i)->Segment == NO_SUBSEGMENTS)
            {
                if(((CurveThread*)DependingList->at(i)->Thread)->isFinished())
                    ((CurveThread*)DependingList->at(i)->Thread)->start();
            }
            else
            {
                for(int k = 0 ; k < NumThreads ; k ++)
                {
                    CurveInformationStruct *CurveSegmentInfo = (DependingList->at(i)->SubSegmentInformation)[k];

                    if(CurveSegmentInfo->Thread->isFinished())
                    {
                        CurveSegmentInfo->Thread->start();
                    }
                }

            }
        }
        VariabelMdl->valueChange();
        return;
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

    for(int i = 0 ; i < DependingList->count() ; i ++)
    {
        if(DependingList->at(i)->Segment == NO_SUBSEGMENTS)
        {
            DependingList->at(i)->MarkerPos = DblVal;
            if(((CurveThread*)DependingList->at(i)->Thread)->isFinished())
                ((CurveThread*)DependingList->at(i)->Thread)->start();
        }
        else
        {
            for(int k = 0 ; k < NumThreads ; k ++)
            {
                CurveInformationStruct *CurveSegmentInfo = (DependingList->at(i)->SubSegmentInformation)[k];
                CurveSegmentInfo->MarkerPos = DblVal;
                if(CurveSegmentInfo->Thread->isFinished())
                {
                    CurveSegmentInfo->Thread->start();
                }
            }

        }

    }
    return;
}

void MainWindow::on_ExpressionListView_doubleClicked(const QModelIndex &index)
{
    ParserX *Expression = ExpressionMdl->getExpression(index);
    ExpressionDialog Dlg(this, Expression);
    Dlg.setModal(true);
    Dlg.exec();

    ExpressionMdl->setData(index, QString(Expression->GetExpr().c_str()),Qt::EditRole);

    delete Expression;
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

    }
    ::close(INFILE);

    return;
}

void MainWindow::store(QString FilePath)
{
    char ExpressionBuffer[1024*1024];
    char *ptr=ExpressionBuffer;
    for(int i = 0 ; i < ExpressionMdl->rowCount() ; i ++ )
    {
        const char *Expression = QString(ExpressionMdl->data(ExpressionMdl->index(i), Qt::DisplayRole).toString()).toStdString().c_str();
        memcpy((void*)ptr, Expression, strlen(Expression)+1);
        ptr += (long)strlen(Expression)+1;
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

    for(int i = 0 ; i < VariabelMdl->rowCount() ; i ++)
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

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dlg(this, QString("Laden"),QString("data/"));
    dlg.setModal(true);

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            load(dlg.selectedFiles().at(0));
        }
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dlg(this, QString("Speichern"),QString("data/"));
    dlg.setModal(true);

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            setWindowTitle(dlg.selectedFiles().at(0));

            QString Filename = dlg.selectedFiles().at(0);
            store(Filename);
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
    if(CurveInfo->Segment == NO_SUBSEGMENTS)
    {
        if(CurveInfo->CurveType != CURVE_TYPE_COMPLEX)
        {
            CurveInformationList.removeOne(CurveInfo);

            CurveInfo->Thread->terminate();
            delete CurveInfo->Thread;
            CurveInfo->Curve->detach();
            delete CurveInfo->Curve;
            CurveInfo->Marker->detach();
            delete CurveInfo->Marker;
            QWidget *Wdg = (QWidget*) CurveInfo->Plot->parent();
            delete CurveInfo->Plot;
            delete Wdg;
        }
        else
        {
            CurveInformationList.removeOne(CurveInfo);
            CurveInfo->Thread->terminate();
            delete CurveInfo->Thread;
            CurveInfo->Curve->detach();
            delete CurveInfo->Curve;
            CurveInfo->Marker->detach();
            delete CurveInfo->Marker;
            CurveInfo->Plot->replot();
        }
    }
    else
    {
        CurveInformationList.removeOne(CurveInfo);
        for(int k = 0 ; k < NumThreads ; k ++)
        {

            CurveInformationStruct *CurveSegmentInfo = CurveInfo->SubSegmentInformation[k];
            CurveSegmentInfo->Thread->terminate();
            delete CurveSegmentInfo->Thread;
        }
        CurveInfo->Curve->detach();
        delete CurveInfo->Curve;
        QWidget *Wdg = (QWidget*) CurveInfo->Plot->parent();
        delete CurveInfo->Plot;
        delete Wdg;

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

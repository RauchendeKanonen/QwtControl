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


    /*ui->CurveSetListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->CurveSetListView->setDragEnabled(true);
    ui->CurveSetListView->setAcceptDrops(true);
    ui->CurveSetListView->setDropIndicatorShown(true);*/

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
    double Val;
    QString Name;
    VarDialog Dlg(this, &Val, &Name);
    Dlg.setModal(true);
    Dlg.exec();

    if(!Dlg.exec())
        return;

    QString ValueString;
    ValueString.sprintf("%f", Val);

    QModelIndex index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->insertRows(0, 1, (const QModelIndex &)index);
    index = VariabelMdl->index(0, 0, QModelIndex());
    VariabelMdl->setData(index, QString(Name + QString(" = ") + ValueString),Qt::EditRole);
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

        QwtRootLocusCurve *Curve = new QwtRootLocusCurve(Expression, Evinfo);

        Curve->setSymbol(QwtSymbol());
        connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>)), Curve, SLOT(valueChangeSlot(QPair<QString,double>)));
        connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Curve, SLOT(markerChangeSlot(QPair<QString,double>)));
        Curve->attach(ui->qwtPlot);
        Curve->setPen(QPen(Color));
        Curve->setSymbol(QwtSymbol( QwtSymbol::Rect,
                                              QColor(Color), QColor(Color), QSize( 2, 2 ) ));
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


        Curve->setSymbol(QwtSymbol());
        connect(this, SIGNAL(valueChangeSignal(QPair<QString,double>)), Curve, SLOT(valueChangeSlot(QPair<QString,double>)));
        connect(this, SIGNAL(markerChangeSignal(QPair<QString,double>)), Curve, SLOT(markerChangeSlot(QPair<QString,double>)));
        Curve->attach(StepRespDialog->getPlot());
        Curve->setPen(QPen(Color));
        Curve->setSymbol(QwtSymbol( QwtSymbol::Rect,
                                              QColor(Color), QColor(Color), QSize( 2, 2 ) ));
        Curve->start();

        CurveList.append((QwtPlotItem*)Curve);
    }
    emitAllValues();
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
        parameterChange(VarName, *Val);
    }
}


void MainWindow::parameterChange(QString VarName, double DblVal)
{
    double *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        *Val = DblVal;
        VariabelMdl->valueChange();
        QPair<QString,double> VarPair;
        VarPair.first = VarName;
        VarPair.second = DblVal;
        emit valueChangeSignal(VarPair);
    }
}

void MainWindow::markerChange(QString VarName, double DblVal)
{
    double *Val = VariabelMdl->getVarValuePtr(VarName);

    if(Val != NULL)
    {
        *Val = DblVal;
        VariabelMdl->valueChange();
        QPair<QString,double> VarPair;
        VarPair.first = VarName;
        VarPair.second = DblVal;
        emit markerChangeSignal(VarPair);
        emit valueChangeSignal(VarPair);
    }
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
    double *Val = VariabelMdl->getVarValuePtr(Name);

    VarDialog Dlg(this, Val, &Name);
    Dlg.setModal(true);
    Dlg.exec();

    QString ValueString;
    ValueString.sprintf("%f", *Val);
    VariabelMdl->setData(index, QString(Name + QString(" = ") + ValueString),Qt::EditRole);
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
    QwtPlotItem *CurveItem = CurveList.at(index.row());

    QMenu ExpressionMenu;
    ExpressionMenu.addAction(QString("delete"));

    QAction *happened = ExpressionMenu.exec(globalPos);

    if(happened == NULL)
        return;

    if(happened->text() == QString("delete"))
    {

    }

}

void MainWindow::on_CurveListView_doubleClicked(const QModelIndex &index)
{
    QwtPlotItem *Curve = CurveList.at(index.row());
    QwtPlot *Plot = Curve->plot();


    /*RangeSelectorDialog Dlg(this, QString(CurveInfo->IndepVarName));
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
    IndependentMarkerSliderDialog->raise();*/

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


    QString FunctionName = File;
}


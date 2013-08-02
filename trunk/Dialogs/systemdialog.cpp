#include "systemdialog.h"
#include "ui_systemdialog.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "controlsystemtracker.h"

SystemDialog::SystemDialog(QWidget *parent, QString *ExpressionA) :
    QDialog(parent),
    ui(new Ui::SystemDialog)
{
    pParent = parent;
    pExpression = ExpressionA;
    ui->setupUi(this);


    ControlExpression Exp(*pExpression);
    QImage Img = Exp.picture();
    Scene = new QGraphicsScene();
    ui->EquationGraphicsView->setScene(Scene);
    Item = Scene->addPixmap(QPixmap::fromImage(QImage(Img)));
    ui->ExpressionEdit->setText(*pExpression);
}




SystemDialog::~SystemDialog()
{
    delete ui;
    delete Scene;
}

void SystemDialog::on_buttonBox_accepted()
{
    QString Expression = ui->ExpressionEdit->text();
    if(Expression.count() == 0)
        return;
    Expression.replace("%i", "I");
    Expression.remove("%");
    Expression.remove("\n");
    *pExpression = Expression;
}

void SystemDialog::on_ExpressionEdit_textChanged(const QString &arg1)
{
    ControlExpression Exp(arg1);

    QImage Img = Exp.picture();
    QGraphicsPixmapItem *oldItem = Item;
    Scene->removeItem(oldItem);
    Item = Scene->addPixmap(QPixmap::fromImage(Img));

    if(oldItem)
        delete oldItem;
}



void SystemDialog::on_pushButtonGenSystem_clicked()
{
    QString Expression = ui->ExpressionEdit->text();
    if(Expression.count() == 0)
        return;
    Expression.replace("%i", "I");
    Expression.remove("%");
    Expression.remove("\n");

    QString ExpressionName = ui->NameEdit->text();

    QModelIndex TransferFunctionIndex = ((MainWindow*)pParent)->insertExpression(ExpressionName+QString("_tf:")+Expression);

    if(ui->checkBoxRootLocus->isChecked())
        ((MainWindow*)pParent)->on_ExpressionListView_customContextMenuRequested(TransferFunctionIndex, "draw numeric Root Locus");

    if(ui->checkBoxBode->isChecked())
        ((MainWindow*)pParent)->on_ExpressionListView_customContextMenuRequested(TransferFunctionIndex, "draw Bode");


    QModelIndex StepResponseFunctionIndex = ((MainWindow*)pParent)->insertExpression(ExpressionName+QString("_sr:")+Expression+QString("*1/s"));

    if(ui->checkBoxStepResponse->isChecked())
        ((MainWindow*)pParent)->on_ExpressionListView_customContextMenuRequested(StepResponseFunctionIndex, ("draw Response"));

}

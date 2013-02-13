#include "closeloopdialog.h"
#include "ui_closeloopdialog.h"

CloseLoopDialog::CloseLoopDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseLoopDialog)
{
    ui->setupUi(this);
}

CloseLoopDialog::~CloseLoopDialog()
{
    delete ui;
}

void CloseLoopDialog::on_buttonBox_accepted()
{
    ClosedLoopExpression = QString("(")+ui->ContollerLineEdit->text()+QString("*")+ui->PlantLineEdit->text()+QString(")")
            +QString("/(1+")+ui->ContollerLineEdit->text()+QString("*")+ui->PlantLineEdit->text()+ui->FedBackLineEdit->text();
    ClosedLoopExpression += QString(")");
}

QString CloseLoopDialog::getEquation(void)
{
    return ClosedLoopExpression;
}


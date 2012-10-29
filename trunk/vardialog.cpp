#include "vardialog.h"
#include "ui_vardialog.h"
#include <iostream>
VarDialog::VarDialog(QWidget *parent, double *Val, QString *Name) :
    QDialog(parent),
    ui(new Ui::VarDialog)
{
    pVal = Val;
    pName = Name;
    ui->setupUi(this);
    ui->ExpressionEdit->setText(*Name);
    QString ValueString;
    ValueString.sprintf("%f", *Val);
    ui->ValueEdit->setText(ValueString);
}

VarDialog::~VarDialog()
{
    delete ui;
}

void VarDialog::on_buttonBox_accepted()
{
    QString Expression = ui->ExpressionEdit->text();
    if(Expression.count() == 0)
        return;
    bool Ok;
    double Value = (double)ui->ValueEdit->text().toDouble(&Ok);

    if(Ok)
    {
        *pVal = Value;
        *pName = QString(ui->ExpressionEdit->text().toStdString().c_str());
    }
    accept();
}

#include "expressiondialog.h"
#include "ui_expressiondialog.h"
#include <iostream>

ExpressionDialog::ExpressionDialog(QWidget *parent, QString *ExpressionA) :
    QDialog(parent),
    ui(new Ui::ExpressionDialog)
{
    pExpression = ExpressionA;
    ui->setupUi(this);
    ui->ExpressionEdit->setText(*pExpression);
}




ExpressionDialog::~ExpressionDialog()
{
    delete ui;
}

void ExpressionDialog::on_buttonBox_accepted()
{
    QString Expression = ui->ExpressionEdit->text();
    if(Expression.count() == 0)
        return;
    *pExpression = Expression;
}


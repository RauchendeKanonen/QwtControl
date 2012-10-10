#include "expressiondialog.h"
#include "ui_expressiondialog.h"
#include <iostream>

ExpressionDialog::ExpressionDialog(QWidget *parent, ParserX *muFunctionParserA) :
    QDialog(parent),
    ui(new Ui::ExpressionDialog)
{
    ui->setupUi(this);


    muComplexFunctionParser = muFunctionParserA;

    try
    {
        if(muComplexFunctionParser->ValidNameChars() != NULL)
            ui->ExpressionEdit->setText(QString(muComplexFunctionParser->GetExpr().c_str()));
    }
    catch (mup::ParserError &e)
    {

    }
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

    Expression.remove("%");
    Expression.remove("\n");
    muComplexFunctionParser->SetExpr(Expression.toStdString().c_str());
}

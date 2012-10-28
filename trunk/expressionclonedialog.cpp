#include "expressionclonedialog.h"
#include "ui_expressionclonedialog.h"

ExpressionCloneDialog::ExpressionCloneDialog(QWidget *parent, QString ExpressionA) :
    QDialog(parent),
    ui(new Ui::ExpressionCloneDialog)
{
    Expression = ExpressionA;
    ui->setupUi(this);
}

ExpressionCloneDialog::~ExpressionCloneDialog()
{
    delete ui;
}

void ExpressionCloneDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ExpressionCloneDialog::on_buttonBox_accepted()
{

}

QString ExpressionCloneDialog::getNewExpression(void)
{
    return Expression.replace(ui->ToReplaceLineEdit->text(),ui->WithThisLineEdit->text());
}

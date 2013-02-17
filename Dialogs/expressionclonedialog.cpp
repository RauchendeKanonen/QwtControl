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
    QString Enclosed = QString("(") + ui->WithThisLineEdit->text() + QString(")");
    return Expression.replace(ui->ToReplaceLineEdit->text(), Enclosed);
}


void ExpressionCloneDialog::on_BilinearPushButton_clicked()
{
    ui->WithThisLineEdit->setText("2/T*(z-1)/(z+1)");
}

void ExpressionCloneDialog::on_EulerFwPushButton_clicked()
{
    ui->WithThisLineEdit->setText("(z-1)/T");
}

void ExpressionCloneDialog::on_EulerBwPushButton_clicked()
{
    ui->WithThisLineEdit->setText("(z-1)/(T*z)");
}

void ExpressionCloneDialog::on_BilinearPushButtonToS_clicked()
{
    ui->WithThisLineEdit->setText("(1+T/2*s)/(1-T/2*s)");
}

void ExpressionCloneDialog::on_EulerFwPushButtonToS_clicked()
{
    ui->WithThisLineEdit->setText("1+T*s");
}

void ExpressionCloneDialog::on_EulerBwPushButtonToS_clicked()
{
    ui->WithThisLineEdit->setText("1/(1-T*s)");
}

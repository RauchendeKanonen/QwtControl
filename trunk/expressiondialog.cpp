#include "expressiondialog.h"
#include "ui_expressiondialog.h"


ExpressionDialog::ExpressionDialog(QWidget *parent, QString *ExpressionA) :
    QDialog(parent),
    ui(new Ui::ExpressionDialog)
{
    pExpression = ExpressionA;
    ui->setupUi(this);


    ControlExpression Exp(*pExpression);
    QImage Img = Exp.picture();
    Scene = new QGraphicsScene();
    ui->EquationGraphicsView->setScene(Scene);
    Item = Scene->addPixmap(QPixmap::fromImage(QImage(Img)));
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
    Expression.remove("%");
    Expression.remove("\n");
    *pExpression = Expression;
}


void ExpressionDialog::on_ExpressionEdit_textChanged(const QString &arg1)
{
    ControlExpression Exp(arg1);
    QString Src = Exp.cSourceString();
    QImage Img = Exp.picture();
    Scene->clear();
    Scene->addPixmap(QPixmap::fromImage(Img));
}

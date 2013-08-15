#include "discretesystemdialog.h"
#include "ui_discretesystemdialog.h"
#include <QMessageBox>
#include "mainwindow.h"



DiscreteSystemDialog::DiscreteSystemDialog(QWidget *parent, QString *ExpressionA) :
    QDialog(parent),
    ui(new Ui::DiscreteSystemDialog)
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




DiscreteSystemDialog::~DiscreteSystemDialog()
{
    delete ui;
    delete Scene;
}

void DiscreteSystemDialog::on_buttonBox_accepted()
{
    QString Expression = ui->ExpressionEdit->text();
    if(Expression.count() == 0)
        return;
    Expression.replace("%i", "I");
    Expression.remove("%");
    Expression.remove("\n");
    *pExpression = Expression;
}

ex DiscreteSystemDialog::assembleDifferenceEquation(ex Rational, symbol s)
{
    symbol x("x");
    symbol y("y");
    symbol *y0;
    ex Numerator = Rational.numer();
    ex DeNumerator = Rational.denom();

    ex Polynome = DeNumerator;

    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    ex Poly = Polynome.expand();
    ex YTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;
        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",y.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",y.get_name().c_str());

        symbol *ydq = new symbol(SymbolStr.toStdString());
        if(i==0)
            y0 = ydq;
        YTerm += *ydq*Poly.coeff(s,i);
    }


    Polynome = Numerator;
    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    Poly = Polynome.expand();
    ex XTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;


        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",x.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",x.get_name().c_str());

        symbol *xdq = new symbol(SymbolStr.toStdString());
        XTerm += *xdq*Poly.coeff(s,i);
    }

    ex DifferenceEquation = YTerm==XTerm;

    DifferenceEquation = lsolve(DifferenceEquation, *y0);
    return DifferenceEquation;
}

void DiscreteSystemDialog::on_ExpressionEdit_textChanged(const QString &arg1)
{
    ControlExpression Exp(arg1);

    QImage Img = Exp.picture();
    QGraphicsPixmapItem *oldItem = Item;
    Scene->removeItem(oldItem);
    Item = Scene->addPixmap(QPixmap::fromImage(Img));

    if(oldItem)
        delete oldItem;
}

void DiscreteSystemDialog::on_pushButton_clicked()
{
    ui->InverseZlineEdit->setText(inverseZTransform(ui->ExpressionEdit->text()));
}


void DiscreteSystemDialog::on_pushButtonGenSystem_clicked()
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



    GiNaC::parser reader;
    GiNaC::ex GExpression;
    try
    {
        GExpression = reader(ui->ExpressionEdit->text().toStdString());
    }
    catch (exception &p)
    {
        QMessageBox Box;
        Box.setText(p.what());
        Box.setModal(true);
        Box.exec();
        return;
    }

    GiNaC::symtab table = reader.get_syms();
    GiNaC::symbol z = table.find("z") != table.end() ?
                GiNaC::ex_to<GiNaC::symbol>(table["z"]) : GiNaC::symbol("z");
    ex DifferenceEq = assembleDifferenceEquation(GExpression, z);
    ostringstream DiffStr;
    DiffStr << DifferenceEq;


    QString DifferenceEquation(DiffStr.str().c_str());


    QModelIndex StepResponseFunctionIndex = ((MainWindow*)pParent)->insertExpression(ExpressionName+QString("_dsr:")+DifferenceEquation);

    if(ui->checkBoxStepResponse->isChecked())
        ((MainWindow*)pParent)->on_ExpressionListView_customContextMenuRequested(StepResponseFunctionIndex, ("draw discrete step Response"));
}

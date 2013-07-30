#ifndef DISCRETESYSTEMDIALOG_H
#define DISCRETESYSTEMDIALOG_H
#include <QDialog>
#include <iostream>

#include <iostream>
#include <string>
#include <ginac/ginac.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "controlexpression.h"
using namespace std;
using namespace GiNaC;
using namespace std;


namespace Ui {
class DiscreteSystemDialog;
}

class DiscreteSystemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiscreteSystemDialog(QWidget *parent, QString *ExpressionA);
    ~DiscreteSystemDialog();
    ex assembleDifferenceEquation(ex Rational, symbol s);
private slots:
    void on_buttonBox_accepted();

    void on_ExpressionEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();
    void on_pushButtonGenSystem_clicked();

private:
    QWidget *pParent;
    QPixmap *ActImage;
    QGraphicsPixmapItem *Item;
    QGraphicsScene *Scene;
    QString *pExpression;
    Ui::DiscreteSystemDialog *ui;
};

#endif // DISCRETESYSTEMDIALOG_H

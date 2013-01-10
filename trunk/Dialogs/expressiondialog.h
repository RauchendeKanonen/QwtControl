#ifndef EXPRESSIONDIALOG_H
#define EXPRESSIONDIALOG_H

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
class ExpressionDialog;
}

class ExpressionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExpressionDialog(QWidget *parent, QString *ExpressionA);
    ~ExpressionDialog();
    
private slots:
    void on_buttonBox_accepted();

    void on_ExpressionEdit_textChanged(const QString &arg1);

private:
    QPixmap *ActImage;
    QGraphicsPixmapItem *Item;
    QGraphicsScene *Scene;
    QString *pExpression;
    Ui::ExpressionDialog *ui;
};

#endif // EXPRESSIONDIALOG_H

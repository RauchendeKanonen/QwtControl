#ifndef SystemDialog_H
#define SystemDialog_H

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
class SystemDialog;
}

class SystemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemDialog(QWidget *parent, QString *ExpressionA);
    ~SystemDialog();

private slots:
    void on_buttonBox_accepted();
    void on_ExpressionEdit_textChanged(const QString &arg1);
    void on_pushButtonGenSystem_clicked();

private:
    QWidget *pParent;
    QPixmap *ActImage;
    QGraphicsPixmapItem *Item;
    QGraphicsScene *Scene;
    QString *pExpression;
    Ui::SystemDialog *ui;
};

#endif // SystemDialog_H

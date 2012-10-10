#ifndef VARDIALOG_H
#define VARDIALOG_H

#include <QDialog>
#include <iostream>
#include "mpParser.h"
using namespace std;
using namespace mup;

namespace Ui {
class VarDialog;
}

class VarDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VarDialog(QWidget *parent, Value *Val, QString *Name);
    ~VarDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Value *pVal;
    QString *pName;
    Ui::VarDialog *ui;
};

#endif // EXPRESSIONDIALOG_H

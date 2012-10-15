#ifndef EXPRESSIONDIALOG_H
#define EXPRESSIONDIALOG_H

#include <QDialog>
#include <iostream>
#include "mpParser.h"
using namespace std;
using namespace mup;

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

private:
    QString *pExpression;
    Ui::ExpressionDialog *ui;
};

#endif // EXPRESSIONDIALOG_H

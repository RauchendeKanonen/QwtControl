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
    explicit ExpressionDialog(QWidget *parent, ParserX *muFunctionParserA);
    ~ExpressionDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    ParserX *muComplexFunctionParser;
    Ui::ExpressionDialog *ui;
};

#endif // EXPRESSIONDIALOG_H

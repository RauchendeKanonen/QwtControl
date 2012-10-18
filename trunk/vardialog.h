#ifndef VARDIALOG_H
#define VARDIALOG_H

#include <QDialog>
#include <iostream>

using namespace std;


namespace Ui {
class VarDialog;
}

class VarDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VarDialog(QWidget *parent, double *Val, QString *Name);
    ~VarDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    double *pVal;
    QString *pName;
    Ui::VarDialog *ui;
};

#endif // EXPRESSIONDIALOG_H

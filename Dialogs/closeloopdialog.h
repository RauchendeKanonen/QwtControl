#ifndef CLOSELOOPDIALOG_H
#define CLOSELOOPDIALOG_H

#include <QDialog>

namespace Ui {
class CloseLoopDialog;
}

class CloseLoopDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CloseLoopDialog(QWidget *parent = 0);
    QString getEquation(void);
    ~CloseLoopDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::CloseLoopDialog *ui;
    QString ClosedLoopExpression;
};

#endif // CLOSELOOPDIALOG_H

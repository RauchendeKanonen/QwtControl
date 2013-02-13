#ifndef EXPRESSIONCLONEDIALOG_H
#define EXPRESSIONCLONEDIALOG_H

#include <QDialog>

namespace Ui {
    class ExpressionCloneDialog;
}

class ExpressionCloneDialog : public QDialog {
    Q_OBJECT
public:
    ExpressionCloneDialog(QWidget *parent, QString Expression);
    ~ExpressionCloneDialog();
    QString getNewExpression(void);
protected:
    void changeEvent(QEvent *e);

private:
    QString Expression;
    Ui::ExpressionCloneDialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_BilinearPushButton_clicked();
    void on_EulerFwPushButton_clicked();
    void on_EulerBwPushButton_clicked();
    void on_BilinearPushButtonToS_clicked();
    void on_EulerFwPushButtonToS_clicked();
    void on_EulerBwPushButtonToS_clicked();
};

#endif // EXPRESSIONCLONEDIALOG_H

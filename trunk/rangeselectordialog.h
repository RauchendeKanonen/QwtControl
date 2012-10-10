#ifndef RANGESELECTORDIALOG_H
#define RANGESELECTORDIALOG_H

#include <QDialog>

namespace Ui {
class RangeSelectorDialog;
}

class RangeSelectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    QPointF getRange(void);
    explicit RangeSelectorDialog(QWidget *parent, QString VarName);
    ~RangeSelectorDialog();
    QString getVarName(void);
    double getResolution(void);
    void setRange(QPointF RangeA);
    void setResolution(double Resolution);
private:
    Ui::RangeSelectorDialog *ui;
};

#endif // RANGESELECTORDIALOG_H

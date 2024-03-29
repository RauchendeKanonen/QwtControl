#ifndef RANGESELECTORDIALOG_H
#define RANGESELECTORDIALOG_H
#include "colorlisteditor.h"
#include <QDialog>

namespace Ui {
class RangeSelectorDialog;
}

class RangeSelectorDialog : public QDialog
{
    Q_OBJECT
public slots:
    void variableSelected(const QString Index);
public:
    QPointF getRange(void);
    explicit RangeSelectorDialog(QWidget *parent, QString VarName);
    RangeSelectorDialog(QWidget *parent, QStringList VarNames);
    ~RangeSelectorDialog();
    QString getVarName(void);
    double getResolution(void);
    void setRange(QPointF RangeA);
    void setResolution(double Resolution);
    void setColor(QColor Color);
    QColor getColor(void);
private:
    QComboBox *VarComboBox;
    ColorListEditor *ColorCombo;
    Ui::RangeSelectorDialog *ui;
};

#endif // RANGESELECTORDIALOG_H

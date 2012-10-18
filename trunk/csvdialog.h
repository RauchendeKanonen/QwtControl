#ifndef CSVDIALOG_H
#define CSVDIALOG_H

#include <QDialog>
#include "csv.h"
namespace Ui {
class CSVDialog;
}

class CSVDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSVDialog(QWidget *parent = 0, csv *CSVParserA=0);
    ~CSVDialog();
    void updateCSVPreview(void);
private slots:
    void on_DelimiterLineEdit_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    csv *CSVParser;
    Ui::CSVDialog *ui;
};

#endif // CSVDIALOG_H

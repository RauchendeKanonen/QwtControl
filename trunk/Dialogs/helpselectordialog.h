#ifndef HELPSELECTORDIALOG_H
#define HELPSELECTORDIALOG_H

#include <QDialog>

namespace Ui {
class HelpSelectorDialog;
}

class HelpSelectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HelpSelectorDialog(QWidget *parent = 0, QStringList HelpEntries = QStringList());
    ~HelpSelectorDialog();
    QString getSelectedHelpEntry(void);
    
private:
    Ui::HelpSelectorDialog *ui;
};

#endif // HELPSELECTORDIALOG_H

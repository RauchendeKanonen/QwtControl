#include "helpselectordialog.h"
#include "ui_helpselectordialog.h"

HelpSelectorDialog::HelpSelectorDialog(QWidget *parent, QStringList HelpEntries) :
    QDialog(parent),
    ui(new Ui::HelpSelectorDialog)
{
    ui->setupUi(this);
    for(int i = 0 ; i < HelpEntries.count() ; i ++ )
        ui->listWidget->insertItem(0, HelpEntries.at(i));
}

HelpSelectorDialog::~HelpSelectorDialog()
{
    delete ui;
}

QString HelpSelectorDialog::getSelectedHelpEntry(void)
{
    if(ui->listWidget->selectedItems().count())
        return ui->listWidget->selectedItems().at(0)->text();

    return QString();
}

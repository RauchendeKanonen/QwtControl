#include "templatedialog.h"
#include "ui_templatedialog.h"
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
TemplateDialog::TemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemplateDialog)
{
    ui->setupUi(this);
    updateTemplates();
}

void TemplateDialog::updateTemplates(void)
{
    ui->listWidgetTemplates->clear();
    QDir Dir(QString("Templates"));

    QStringList filters;
    filters << "*.deq";
    Dir.setNameFilters(filters);

    QStringList Templates = Dir.entryList();


    for(int i = 0 ; i < Templates.count() ; i++)
        ui->listWidgetTemplates->insertItem(0, Templates.at(i));
}


TemplateDialog::~TemplateDialog()
{
    delete ui;
}

void TemplateDialog::on_listWidgetTemplates_activated(const QModelIndex &index)
{
    QDir Dir(QString("Templates"));
    QString Filename = ui->listWidgetTemplates->item(index.row())->text();
    QString FilenameDescr = Filename;
    FilenameDescr.append(".descr");
    char Buffer[1024*1024];
    memset(Buffer, 0, sizeof(Buffer));
    int BufSize;

    ui->textEditExplanation->clear();
    ui->textEditExpression->clear();

    QFile File(QString("Templates/")+Filename);

    if (File.open(QFile::ReadOnly))
    {
        File.open(QIODevice::ReadOnly);
        QDataStream in(&File);
        BufSize = in.readRawData((char*) Buffer, File.size());
        File.close();
    }
    else
        return;
    ui->textEditExpression->setText(Buffer);

    QFile DescrFile(QString("Templates/")+FilenameDescr);

    if (DescrFile.open(QFile::ReadOnly))
    {
        DescrFile.open(QIODevice::ReadOnly);
        QDataStream in(&DescrFile);
        BufSize = in.readRawData((char*) Buffer, DescrFile.size());
        DescrFile.close();
    }
    else
        return;

    ui->textEditExplanation->setText(Buffer);

}

QString TemplateDialog::getExpression(void)
{
    return  ui->textEditExpression->toPlainText();
}

void TemplateDialog::on_pushButtonNew_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Specify new Name"),
                                         tr("new Template:"), QLineEdit::Normal,
                                         "", &ok);
    if(ok)
    {
        QString Command("touch Templates/");
        Command += text+".deq";
        system(Command.toStdString().c_str());
        Command += text+".descr";
        system(Command.toStdString().c_str());
    }
    updateTemplates();
}

void TemplateDialog::on_pushButtonStore_clicked()
{
    QList <QListWidgetItem*> SelectedItems = ui->listWidgetTemplates->selectedItems();



    void *Buffer = malloc(1024*1024);
    int BufSize = ui->textEditExpression->toPlainText().size()+1;
    memset(Buffer, 0 , BufSize);

    strcpy((char*)Buffer, ui->textEditExpression->toPlainText().toStdString().c_str());

    QString Path = QString("Templates/") + SelectedItems.at(0)->text();

    QFile OutFile(Path);

    if (OutFile.open(QFile::WriteOnly))
    {
        OutFile.open(QIODevice::WriteOnly);
        QDataStream OutStream(&OutFile);
        OutStream.writeRawData((const char *)Buffer, BufSize);
        OutFile.close();
    }
    else
    {
        free(Buffer);
        return;
    }

    if(ui->textEditExplanation->toPlainText().count())
    {
        Path.append(".descr");
        QFile DescrFile(Path);
        memset(Buffer, 0 , BufSize);
        strcpy((char*)Buffer, ui->textEditExplanation->toPlainText().toStdString().c_str());
        BufSize = ui->textEditExplanation->toPlainText().count()+1;

        if (DescrFile.open(QFile::WriteOnly))
        {
            QDataStream OutStream(&DescrFile);
            OutStream.writeRawData((const char *)Buffer, BufSize);
            DescrFile.close();
        }
        else
        {
            free(Buffer);
            return;
        }
    }
    free(Buffer);
}

void TemplateDialog::on_pushButtonDelete_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete Template");
    msgBox.setText("Delete all selected templates?");
    msgBox.addButton(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::No)
        return;

    QList <QListWidgetItem*> SelectedItems = ui->listWidgetTemplates->selectedItems();

    for(int i = 0 ; i < SelectedItems.count() ; i ++ )
    {
        QString Command("rm Templates/");
        Command += SelectedItems.at(i)->text();
        system(Command.toStdString().c_str());
        Command += ".descr";
        system(Command.toStdString().c_str());
    }
    updateTemplates();

    ui->textEditExplanation->clear();
    ui->textEditExpression->clear();

}

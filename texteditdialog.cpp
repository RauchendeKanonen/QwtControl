#include "texteditdialog.h"
#include "ui_texteditdialog.h"

TextEditDialog::TextEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEditDialog)
{
    ui->setupUi(this);
}

TextEditDialog::~TextEditDialog()
{
    delete ui;
}


void TextEditDialog::setText(QString Text)
{
    ui->textEdit->setText(Text);
}

QString TextEditDialog::getText(void)
{
    return ui->textEdit->toPlainText();
}

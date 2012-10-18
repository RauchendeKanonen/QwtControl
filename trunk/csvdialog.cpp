#include "csvdialog.h"
#include "ui_csvdialog.h"

CSVDialog::CSVDialog(QWidget *parent, csv *CSVParserA) :
    QDialog(parent),
    ui(new Ui::CSVDialog)
{
    CSVParser = CSVParserA;
    ui->setupUi(this);
    ui->DelimiterLineEdit->setText(QString(":"));
    updateCSVPreview();
}

CSVDialog::~CSVDialog()
{
    delete ui;
}

void CSVDialog::updateCSVPreview(void)
{
    CSVParser->setDelimiter(ui->DelimiterLineEdit->text().toStdString().c_str()[0]);
    CSVParser->readCSV(5);
    QPolygonF P = CSVParser->getData();
    QString Preview;

    ui->DataPreviewLabel->setText(QString("Error"));
    for(int i = 0 ; i < P.count() ; i ++ )
    {
        QString Line;
        Line.sprintf("%f %f\n", P.at(i).x(), P.at(i).y());
        Preview.append(Line);
    }
    ui->DataPreviewLabel->setText(Preview);
}

void CSVDialog::on_DelimiterLineEdit_textChanged(const QString &arg1)
{
    updateCSVPreview();
}

void CSVDialog::on_buttonBox_accepted()
{
    this->accept();
}

void CSVDialog::on_buttonBox_rejected()
{
    this->reject();
}

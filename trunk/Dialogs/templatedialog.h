#ifndef TEMPLATEDIALOG_H
#define TEMPLATEDIALOG_H

#include <QDialog>
#include <QModelIndex>
namespace Ui {
class TemplateDialog;
}

class TemplateDialog : public QDialog
{
    Q_OBJECT

public:
    void updateTemplates(void);
    QString getExpression(void);
    explicit TemplateDialog(QWidget *parent = 0);
    ~TemplateDialog();

private slots:
    void on_listWidgetTemplates_activated(const QModelIndex &index);

    void on_pushButtonNew_clicked();

    void on_pushButtonStore_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::TemplateDialog *ui;
};

#endif // TEMPLATEDIALOG_H

#ifndef TRANSFORMTYPEDIALOG_H
#define TRANSFORMTYPEDIALOG_H

#include <QDialog>

namespace Ui {
class TransformTypeDialog;
}

class TransformTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransformTypeDialog(QWidget *parent = 0);
    ~TransformTypeDialog();
    int getAlgorithm(void);
private:
    Ui::TransformTypeDialog *ui;
};

#endif // TRANSFORMTYPEDIALOG_H

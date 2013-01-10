#ifndef STEPRESPONSEDIALOG_H
#define STEPRESPONSEDIALOG_H

#include <QDialog>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qmath.h>
#include "qwt_response_curve.h"
#include <QDragMoveEvent>

namespace Ui
{
class StepResponseDialog;
}

class StepResponseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StepResponseDialog(QWidget *parent = 0);
    ~StepResponseDialog();
    void replot(void);
    QwtPlot *getPlot(void);
protected:
    void dropEvent ( QDropEvent * event );
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
private:

    QWidget *pParent;
    Ui::StepResponseDialog *ui;

};

#endif // STEPRESPONSEDIALOG_H

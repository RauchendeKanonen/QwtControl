#ifndef BODEDIALOG_H
#define BODEDIALOG_H

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


using namespace std;

namespace Ui {
class BodeDialog;
}

class BodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BodeDialog(QWidget *parent = 0);
    ~BodeDialog();
    void replot(double StartFrequency, double EndFrequency);
    void setPhase(const double *XData, const double *YData, int Size);
    void setAmplitude(const double *XData, const double *YData, int Size);
    QwtPlot *getPhasePlot(void);
    QwtPlot *getAmplitudePlot(void);
    void replot(void);
private:
    QWidget *pParent;
    Ui::BodeDialog *ui;
};

#endif // BODEDIALOG_H

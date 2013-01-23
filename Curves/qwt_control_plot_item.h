#ifndef QWTCONTROLPLOTITEM_H
#define QWTCONTROLPLOTITEM_H
#include <qwt_plot_item.h>
#include "controlexpression.h"
#include "definitions.h"
#include <QThread>
class QwtControlPlotItem : public QThread, virtual public QwtPlotItem
{
public:
    QwtControlPlotItem();
    virtual EvalInfo *getEvalInfoPtr(void)=0;
    virtual ControlExpression *getExpressionPtr(void)=0;
    virtual QColor getColor(void)=0;
    virtual void   setColor(QColor Col)=0;
    virtual QString typeName(void)=0;
    virtual void stopThread()=0;
};

#endif // QWTCONTROLPLOTITEM_H

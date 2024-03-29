/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_Z_WD_CURVE_H
#define QWT_Z_WD_CURVE_H

#include "qwt_global.h"
#include "qwt_plot_item.h"
#include "qwt_scale_div.h"
#include "QPair"
#include <qwt_plot.h>
#include <QThread>
#include "qwt_control_plot_item.h"

class QPainter;
class QPen;
class QwtScaleMap;
class QwtScaleDiv;

/*!
  \brief A class which draws a coordinate grid

  The QwtZWdCurve class can be used to draw a coordinate grid.
  A coordinate grid consists of major and minor vertical
  and horizontal gridlines. The locations of the gridlines
  are determined by the X and Y scale divisions which can
  be assigned with setXDiv() and setYDiv().
  The draw() member draws the grid within a bounding
  rectangle.
*/

class QWT_EXPORT QwtZWdCurve: public QwtControlPlotItem
{
    Q_OBJECT
public slots:
    void valueChangeSlot(QPair<QString,double> MarkerPair, bool Restart);
public:
    explicit QwtZWdCurve();
    virtual ~QwtZWdCurve();
    virtual int rtti() const;

    void enable(bool tf);
    const QPen &pen() const;

    void setPen(const QPen &p);

    virtual void draw(QPainter *p, 
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRect &rect) const;

    EvalInfo *getEvalInfoPtr(void)
    {
        return NULL;
    }

    ControlExpression *getExpressionPtr(void)
    {
        return NULL;
    }

    QColor getColor(void)
    {
        return pen().color();
    }
    void   setColor(QColor Col)
    {

    }
    QString typeName(void)
    {
        return QString("Z-Plane Wd");
    }

    void stopThread()
    {

    }

    void setup(void)
    {

    }
private:
    void drawLines(QPainter *painter, const QRect &,
        const QwtScaleMap & xMap, const QwtScaleMap &yMap) const;

    class PrivateData;
    PrivateData *d_data;
};

#endif

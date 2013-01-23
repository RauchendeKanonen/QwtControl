/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include <qpainter.h>
#include <qpen.h>
#include "qwt_painter.h"
#include "qwt_text.h"
#include "qwt_scale_map.h"
#include "qwt_scale_div.h"
#include "qwt_z_zeta_curve.h"

class QwtZZetaCurve::PrivateData
{
public:
    PrivateData():
        Enabled(true)
    {
    }

    bool Enabled;

    QPen Pen;
    double Zeta;
};

//! Enables major grid, disables minor grid
QwtZZetaCurve::QwtZZetaCurve():
    QwtPlotItem(QwtText("Grid"))
{
    d_data = new PrivateData;
    d_data->Zeta = 0.5;
    setZ(10.0);
}

//! Destructor
QwtZZetaCurve::~QwtZZetaCurve()
{
    delete d_data;
}

void QwtZZetaCurve::valueChangeSlot(QPair<QString,double> MarkerPair, bool Restart)
{
    if(MarkerPair.first == QString("zeta"))
    {
        d_data->Zeta = MarkerPair.second;

    }

}

//! \return QwtPlotItem::setZeta
void QwtZZetaCurve::setZeta(double ZetaA)
{
    d_data->Zeta = ZetaA;
}



//! \return QwtPlotItem::Rtti_PlotGrid
int QwtZZetaCurve::rtti() const
{
    return QwtPlotItem::Rtti_PlotGrid;
}

/*!
  \brief Enable or disable vertical gridlines
  \param tf Enable (true) or disable

  \sa Minor gridlines can be enabled or disabled with
      enableXMin()
*/
void QwtZZetaCurve::enable(bool tf)
{
    if ( d_data->Enabled != tf )
    {
        d_data->Enabled = tf;
        itemChanged();
    }
}

/*!
  \brief Draw the grid
  
  The grid is drawn into the bounding rectangle such that 
  gridlines begin and end at the rectangle's borders. The X and Y
  maps are used to map the scale divisions into the drawing region
  screen.
  \param painter  Painter
  \param xMap X axis map
  \param yMap Y axis 
  \param canvasRect Contents rect of the plot canvas
*/
void QwtZZetaCurve::draw(QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const
{
    //  draw minor gridlines
    painter->setPen(QwtPainter::scaledPen(QPen(Qt::black)));
    
    if (d_data->Enabled)
    {
        drawLines(painter, canvasRect, xMap, yMap);
    }

}

void QwtZZetaCurve::drawLines(QPainter *painter, const QRect &canvasRect,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap) const
{
    QPolygonF Line;

    double zeta = d_data->Zeta;
    double pi = 3.141;
    double e = 2.718;
    double T = 1.0;
    double a;
    int dots;
    for(int i = 0 ; ; i ++ )
    {
        a = (double)i/100.0;
        double re = exp(-a*T*cos(-pi*zeta/2))*(cos(-a*T*sin(-pi*zeta/2)));
        double im = exp(-a*T*cos(-pi*zeta/2))*sin(-a*T*sin(-pi*zeta/2));
        if(im < 0.0 || i > 10000)
        {
            dots = i-1;
            break;
        }
        Line.append(QPointF(re, im));
    }

    QPolygon DrawLine;
    for(int i = 0 ; i < dots ; i ++ )
    {
        DrawLine.append(QPoint(xMap.transform(Line.at(i).x()), yMap.transform(Line.at(i).y())));
    }
    QwtPainter::drawPolyline(painter, DrawLine);

    DrawLine.clear();
    for(int i = 0 ; i < dots ; i ++ )
    {
        DrawLine.append(QPoint(xMap.transform(Line.at(i).x()), yMap.transform(-Line.at(i).y())));
    }
    QwtPainter::drawPolyline(painter, DrawLine);
}


/*!
  \return the pen for the minor gridlines
  \sa setMinPen(), setMajPen(), setPen()
*/
const QPen &QwtZZetaCurve::pen() const
{ 
    return d_data->Pen;
}
 

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
#include "qwt_zeta_curve.h"

class QwtZetaCurve::PrivateData
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
QwtZetaCurve::QwtZetaCurve():
    QwtPlotItem(QwtText("Grid"))
{
    d_data = new PrivateData;
    d_data->Zeta = 0.5;
    setZ(10.0);
}

//! Destructor
QwtZetaCurve::~QwtZetaCurve()
{
    delete d_data;
}

void QwtZetaCurve::markerChangeSlot(QPair<QString,double> MarkerPair)
{
    if(MarkerPair.first == QString("zeta"))
    {
        d_data->Zeta = MarkerPair.second;
        if(plot())
        {
            plot()->replot();
        }
    }

}

//! \return QwtPlotItem::setZeta
void QwtZetaCurve::setZeta(double ZetaA)
{
    d_data->Zeta = ZetaA;
}



//! \return QwtPlotItem::Rtti_PlotGrid
int QwtZetaCurve::rtti() const
{
    return QwtPlotItem::Rtti_PlotGrid;
}

/*!
  \brief Enable or disable vertical gridlines
  \param tf Enable (true) or disable

  \sa Minor gridlines can be enabled or disabled with
      enableXMin()
*/
void QwtZetaCurve::enable(bool tf)
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
void QwtZetaCurve::draw(QPainter *painter,
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

void QwtZetaCurve::drawLines(QPainter *painter, const QRect &canvasRect,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap) const
{
    const int x1 = canvasRect.left();
    const int x2 = canvasRect.right();
    const int y1 = canvasRect.top();
    const int y2 = canvasRect.bottom();


    double phi = acos(d_data->Zeta);

    double xBegin = xMap.invTransform(x2);
    double yBegin = yMap.invTransform(y2);

    double xEnd = xMap.invTransform(x1);
    double yEnd = yMap.invTransform(y1);



    double l = sqrt(pow(xBegin-xEnd,2)+pow(yBegin-yEnd,2));

    QPolygonF Line;
    Line.append(QPointF(0, 0));

    xEnd = l*-cos(phi);
    yEnd = l*sin(phi);

    Line.append(QPointF(xEnd,yEnd));






    QPolygon DrawLine;
    DrawLine.append(QPoint(xMap.transform(Line.at(0).x()), yMap.transform(Line.at(0).y())));
    DrawLine.append(QPoint(xMap.transform(Line.at(1).x()), yMap.transform(Line.at(1).y())));
    QwtPainter::drawLine(painter, DrawLine.at(0), DrawLine.at(1));


    DrawLine.clear();
    Line.clear();

    Line.append(QPointF(0, 0));
    xEnd = l*-cos(phi);
    yEnd = -l*sin(phi);
    Line.append(QPointF(xEnd,yEnd));

    DrawLine.append(QPoint(xMap.transform(Line.at(0).x()), yMap.transform(Line.at(0).y())));
    DrawLine.append(QPoint(xMap.transform(Line.at(1).x()), yMap.transform(Line.at(1).y())));
    QwtPainter::drawLine(painter, DrawLine.at(0), DrawLine.at(1));

}


/*!
  \return the pen for the minor gridlines
  \sa setMinPen(), setMajPen(), setPen()
*/
const QPen &QwtZetaCurve::pen() const
{ 
    return d_data->Pen;
}
 

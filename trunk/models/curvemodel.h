#ifndef CURVEMODEL_H
#define CURVEMODEL_H


#include <QAbstractListModel>
#include <qstringlist.h>
#include <QColor>
#include <QFlags>
#include <QFlag>
#include "definitions.h"
#include <QMimeData>
#include "qwt_root_locus_curve.h"


class CurveModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CurveModel(QObject *parent = 0);
    void setCurveList(QList<QwtControlPlotItem *> *CurveListA);
    void valueChange(void);
    QwtControlPlotItem *at(int row);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes)const;
    void removeCurve(QwtControlPlotItem *Item);
    void appendCurve(QwtControlPlotItem *Item);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role, QColor *color);
signals:
    
public slots:

private:
    QList <QwtControlPlotItem*> *CurveList;
    
};

#endif // CURVEMODEL_H

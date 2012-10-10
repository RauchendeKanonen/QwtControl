#ifndef CURVEMODEL_H
#define CURVEMODEL_H


#include <QAbstractListModel>
#include <qstringlist.h>
#include <QColor>
#include <QFlags>
#include <QFlag>
#include "mpParser.h"
#include "definitions.h"

using namespace mup;
class CurveModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CurveModel(QObject *parent = 0);
    void setCurveList(QList <CurveInformationStruct*> *CurveListA)
    {
        CurveList = CurveListA;
        dataChanged(index(0),index(CurveList->count()));
    }

    void valueChange(void)
    {
        dataChanged(index(0),index(CurveList->count()));
    }

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
    QList <CurveInformationStruct*> *CurveList;
    
};

#endif // CURVEMODEL_H

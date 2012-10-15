#include "curvemodel.h"

CurveModel::CurveModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int CurveModel::rowCount(const QModelIndex &parent) const
{
    if(parent.row()==-1 || parent.column()==-1)
    {
        int cnt = CurveList->count();
        return cnt;
    }
    return 0;
}

int CurveModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CurveModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= CurveList->size())
        return QVariant();

    if(role == Qt::ForegroundRole)
        return QVariant(CurveList->at(index.row())->Color);

    if (role == Qt::DisplayRole)
    {
        switch(CurveList->at(index.row())->CurveType)
        {
        case CURVE_TYPE_MAGNITUDE:
            return QVariant(QString("Bode Plot"));

        case CURVE_TYPE_COMPLEX:
            return QVariant(QString("Root Locus"));

        case CURVE_TYPE_XY_COMPILED_NUMERICAL:
            return QVariant(QString("Response"));
        default:
            return QVariant();

        }
    }
    else
        return QVariant();
}

QVariant CurveModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    return QString("Curves");
}


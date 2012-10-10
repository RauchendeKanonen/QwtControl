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

    if (role == Qt::DisplayRole)
    {
        if(CurveList->at(index.row())->Segment == NO_SUBSEGMENTS)
        {
            switch(CurveList->at(index.row())->CurveType)
            {
            case CURVE_TYPE_MAGNITUDE:
                return QVariant(QString("Bode Plot"));

            case CURVE_TYPE_COMPLEX:
                return QVariant(QString("Root Locus"));

            case CURVE_TYPE_XY:
                return QVariant(QString("Symbolic Step Response"));
            default:
                return QVariant();

            }
        }
        else
        {
             return QVariant(QString("Numerical Solution of invers-Laplace Transform"));
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


#include "curvemodel.h"

CurveModel::CurveModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void CurveModel::setCurveList(QList <QwtControlPlotItem*> *CurveListA)
{
    CurveList = CurveListA;
    dataChanged(index(0),index(CurveList->count()));
}


void CurveModel::valueChange(void)
{
    dataChanged(index(0),index(CurveList->count()));
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
QwtControlPlotItem *CurveModel::at(int row)
{
    return CurveList->at(row);
}

void CurveModel::appendCurve(QwtControlPlotItem *Item)
{
    CurveList->append(Item);
    valueChange();
}

void CurveModel::removeCurve(QwtControlPlotItem *Item)
{
    CurveList->removeOne(Item);
    valueChange();
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
    {
        return QVariant(CurveList->at(index.row())->getColor());
    }
    if (role == Qt::DisplayRole)
    {
        return CurveList->at(index.row())->typeName();
    }
    else
        return QVariant();
}

QVariant CurveModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    return QString("Curves");
}

Qt::ItemFlags CurveModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index)  | Qt::ItemIsDragEnabled;
}


QStringList CurveModel::mimeTypes() const
{
    QStringList types;
    types<<"application/set-curve";
    return types;
}


QMimeData* CurveModel::mimeData(const QModelIndexList &indexes)const
{
    QModelIndex Index = this->index(indexes.at(0).row(), 0, QModelIndex());

    if(CurveList->count() > Index.row())
    {
        QMimeData *data = new QMimeData();
        QwtControlPlotItem *CurveInfo = CurveList->at(Index.row());


        data->setData("application/set-curve", QByteArray((const char*)&CurveInfo, sizeof(QwtControlPlotItem*)));

        return data;
    }
    return NULL;
}



#include "curvemodel.h"

CurveModel::CurveModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void CurveModel::setCurveList(QList <QwtPlotItem*> *CurveListA)
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
        switch(CurveList->at(index.row())->rtti())
        {
        case (QwtPlotItem::Rtti_PlotUserItem+Rtti_PlotRootLocus):
        {
            QwtRootLocusCurve *Curve = (QwtRootLocusCurve*) CurveList->at(index.row());
            return QVariant(Curve->pen().color());
        }
            break;
        default:
            return QVariant();

        }
    }
    if (role == Qt::DisplayRole)
    {
        switch(CurveList->at(index.row())->rtti())
        {
        case QwtPlotItem::Rtti_PlotUserItem+Rtti_PlotRootLocus:
            return QVariant(QString("Root Locus"));
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

    /*if(CurveList->count() > Index.row())
    {
        QMimeData *data = new QMimeData();
        CurveInformationStruct *CurveInfo = CurveList->at(Index.row());


        data->setData("application/set-curve", QByteArray((const char*)&CurveInfo, sizeof(CurveInformationStruct*)));

        return data;
    }*/
    return NULL;
}

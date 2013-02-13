#include <stdlib.h>
#include "varmodel.h"
#include <QColor>

double *VarModel::getVarValuePtr(QString VarName)
{
    for(int i = 0 ; i < VarList.count() ; i ++ )
    {
        QString ListName = VarList.at(i).left(VarList.at(i).indexOf(" "));


        if(VarName == ListName)
        {
            return ValueList.at(i);
        }
    }
    return NULL;
}

void VarModel::deleteVar(QString VarName)
{
    for(int i = 0 ; i < VarList.count() ; i ++ )
    {
        QString ListName = VarList.at(i).left(VarList.at(i).indexOf(" "));


        if(VarName == ListName)
        {
            removeRow(i);
            break;
        }
    }
}

void VarModel::valueChange(void)
{
    for(int i = 0 ; i < VarList.count() ; i++ )
    {
        int Idx = VarList.at(i).indexOf(" = ")+3;
        QString VarDef = VarList.at(i).left(Idx);
        QString VarVal;
        VarVal.sprintf("%f", *ValueList.at(i));

        VarList.replace(i, VarDef+VarVal);
    }
    dataChanged(this->index(0), this->index(VarList.count()));
}

int VarModel::rowCount(const QModelIndex &parent) const
{
    if(parent.row()==-1 || parent.column()==-1)
    {
        int cnt = VarList.count();
        return cnt;
    }
    return 0;
}

int VarModel::columnCount(const QModelIndex &parent) const
{
    return VarList.count();
}


QStringList VarModel::getVarStringList(void) const
{
    return VarList;
}

QString VarModel::getVarName(const QModelIndex &index) const
{
    if (!index.isValid())
        return QString();

    if (index.row() >= VarList.size())
        return QString();

    int CIdx = VarList.at(index.row()).indexOf(QString(" = "));

    return QString(VarList.at(index.row()).left(CIdx));
}

QVariant VarModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= VarList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return VarList.at(index.row());
    else
        return QVariant();
}

QVariant VarModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    return QString("");
}

Qt::ItemFlags VarModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;


    return QAbstractItemModel::flags(index);
}

bool VarModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QString ValueString = value.toString();
        VarList.replace(index.row(), value.toString());
        double *ValPtr = ValueList.at(index.row());

        int Index = ValueString.indexOf(" = ") + 3;
        double Value = strtod(ValueString.toStdString().c_str()+Index, NULL);
        *ValPtr = Value;
        return true;
    }
    return false;
}

bool VarModel::insertRows(int position, int rows, const QModelIndex &parent)
{

    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        VarList.insert(position, "");
        ValueList.insert(position, new double);
    }
    endInsertRows();

    return true;
}


bool VarModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        if(VarList.count() < position)
            return false;
        VarList.takeAt(position);
        delete ValueList.takeAt(position);
    }
    endRemoveRows();

    return true;
}

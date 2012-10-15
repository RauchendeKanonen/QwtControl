#include "expressionmodel.h"
#include <QColor>


QString ExpressionModel::getExpressionDefinition(const QModelIndex &index)
{
    if (!index.isValid())
        return QString();

    if (index.row() >= ExpressionList.size())
        return QString();

    return ExpressionList.at(index.row());
}

QString ExpressionModel::getExpressionName(const QModelIndex &index)
{
    if (!index.isValid())
        return QString();

    if (index.row() >= ExpressionList.size())
        return QString();


    QString ExpressionStr = ExpressionList.at(index.row());
    int FunctionNameEnd = ExpressionStr.indexOf(':');

    if(FunctionNameEnd > 0)
    {
        ExpressionStr = ExpressionStr.left(FunctionNameEnd);
        return ExpressionStr;
    }

    return QString();
}


ParserX *ExpressionModel::getExpression(const QModelIndex &index)
{
    if (!index.isValid())
        return NULL;

    if (index.row() >= ExpressionList.size())
        return NULL;


    QString ExpressionStr = ExpressionList.at(index.row());
    int FunctionNameEnd = ExpressionStr.indexOf(':');

    if(FunctionNameEnd > 0)
        ExpressionStr = ExpressionStr.right(ExpressionStr.count() - FunctionNameEnd - 1);

    ParserX *Expression = new ParserX();
    Expression->SetExpr(ExpressionStr.toStdString().c_str());
    return Expression;
}


int ExpressionModel::rowCount(const QModelIndex &parent) const
{
    if(parent.row()==-1 || parent.column()==-1)
    {
        int cnt = ExpressionList.count();
        return cnt;
    }
    return 0;
}

int ExpressionModel::columnCount(const QModelIndex &parent) const
{
    return ExpressionList.count();
}

QVariant ExpressionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= ExpressionList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return ExpressionList.at(index.row());
    else
        return QVariant();
}

QVariant ExpressionModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    return QString("");
}

Qt::ItemFlags ExpressionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;


    return QAbstractItemModel::flags(index);
}


bool ExpressionModel::setData(const QModelIndex &index,
                              const QVariant &value, int role, QColor *color)
{
    if (index.isValid() && role == Qt::EditRole)
    {

        ExpressionList.replace(index.row(), value.toString());
        return true;
    }
    return false;
}


bool ExpressionModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        ExpressionList.replace(index.row(), value.toString());
        return true;
    }
    return false;
}

bool ExpressionModel::insertRows(int position, int rows, const QModelIndex &parent)
{

    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        ExpressionList.insert(position, "");
    }
    endInsertRows();

    return true;
}


bool ExpressionModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        if(ExpressionList.count() < position)
            return true;
        ExpressionList.removeAt(position);
    }
    endRemoveRows();

    return true;
}

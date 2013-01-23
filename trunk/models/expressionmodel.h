#ifndef EXPRESSIONMODEL_H
#define EXPRESSIONMODEL_H

#include <QAbstractListModel>
#include <qstringlist.h>
#include <QColor>
#include <QFlags>
#include <QFlag>
#include "controlexpression.h"
#include <QMimeData>
using namespace std;

//!Model for viewing Msgs in the MainView
//!Implemented because the StandardModel consumed high cputime
//!When ading Messages to it
class ExpressionModel : public QAbstractListModel
{
     Q_OBJECT
 private:

    QStringList ExpressionList;


 public:

     ~ExpressionModel()
     {
     }
    ControlExpression *createExpression(const QModelIndex &index, QString IndepVarname);
    QStringList getExpressionStringList(void);
    QString getExpressionName(const QModelIndex &index);
    QString getExpressionDefinition(const QModelIndex &index) const;
    bool setData(const QModelIndex &index,
                               const QVariant &value, int role);
    bool setData(const QModelIndex &index,
                  const void *value, int role);


     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index,
                               const QVariant &value, int role, QColor *color);
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
     void setflags(QModelIndex index, int Flags);
     bool setColor(const QModelIndex &index, QColor *color);
     QMimeData* mimeData(const QModelIndexList &indexes)const;
     QStringList mimeTypes() const;

};

#endif // EXPRESSIONMODEL_H

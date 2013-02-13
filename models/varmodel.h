#ifndef VarModel_H
#define VarModel_H

#include <QAbstractListModel>
#include <qstringlist.h>
#include <QColor>
#include <QFlags>
#include <QFlag>


//!Model for viewing Msgs in the MainView
//!Implemented because the StandardModel consumed high cputime
//!When ading Messages to it
class VarModel : public QAbstractListModel
{
    Q_OBJECT
private:

    QStringList VarList;
    QList <double*> ValueList;

public:

    ~VarModel()
    {
        for(int i = 0 ; i < ValueList.count() ; i ++ )
        {
            delete ValueList.at(i);
        }
    }
    QStringList getVarStringList(void) const;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role);
    void deleteVar(QString VarName);
    void valueChange(void);
    QString getVarName(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void setflags(QModelIndex index, int Flags);
    bool setColor(const QModelIndex &index, QColor *color);
    double *getVarValuePtr(QString VarName);
};

#endif // VarModel_H

#ifndef QPARAMETERSLIDER_H
#define QPARAMETERSLIDER_H

#include <QSlider>
#include <QLabel>
class QParameterSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QParameterSlider(QWidget *parent, QString VarNameA);
    QString GetName(void)
    {
        return VarName;
    }
    QLabel *getLabel(void)
    {
        return ValueLabel;
    }

signals:
    
public slots:

private:
    QString VarName;
    QObject *pParent;
    QLabel *ValueLabel;
};

#endif // QPARAMETERSLIDER_H

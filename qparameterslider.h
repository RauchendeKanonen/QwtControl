#ifndef QPARAMETERSLIDER_H
#define QPARAMETERSLIDER_H

#include <QSlider>

class QParameterSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QParameterSlider(QWidget *parent, QString VarNameA);
    QString GetName(void)
    {
        return VarName;
    }

signals:
    
public slots:

private:
    QString VarName;
    QObject *pParent;
    
};

#endif // QPARAMETERSLIDER_H

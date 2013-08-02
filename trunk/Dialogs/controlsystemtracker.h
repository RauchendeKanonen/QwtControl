#ifndef CONTROLSYSTEMTRACKER_H
#define CONTROLSYSTEMTRACKER_H

#include <QDialog>

namespace Ui {
class ControlSystemTracker;
}

class ControlSystemTracker : public QDialog
{
    Q_OBJECT
    
public:
    explicit ControlSystemTracker(QWidget *parent = 0);
    ~ControlSystemTracker();
    
private:
    Ui::ControlSystemTracker *ui;
public slots:
    void SystemProperty(double Value, QString PropertyName);
};

#endif // CONTROLSYSTEMTRACKER_H

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //todo
    //bode log - log
    //bode marker
    //n.a. eg var inserted on response

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

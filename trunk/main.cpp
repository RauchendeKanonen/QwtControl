#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //todo
    //variable update
    //insert pi and e as constant
    //z - to - difference equation
    //bode log - log
    //bode marker
    //improve error handling for equation/compilation errors
    //solve problems with linkage of libs (function changed, new compiled but not linked)
    //split in symbolic root locus is buggy
    //symbolic root-locus is nan tough maxima says ist exists

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

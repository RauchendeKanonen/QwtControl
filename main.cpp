#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //todo
    //bode log - log
    //bode marker
    //improve error handling for equation/compilation errors
    //solve problems with linkage of libs (function changed, new compiled but not linked)
    //n.a. eg var inserted on response
    //numeric root locus only on z
    //segmentation fault on indep var s when numeric root

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

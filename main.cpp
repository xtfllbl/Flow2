#include <QtGui/QApplication>
#include "src/qjdmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QJDMainWindow w;
    w.show();
    
    return a.exec();
}

#include <QtGui/QApplication>
#include "src/qjdmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("JDSoft");
    QCoreApplication::setOrganizationDomain("jindisoft.com");
    QCoreApplication::setApplicationName("Flow2");

    QJDMainWindow w;
    w.show();
    
    return a.exec();
}

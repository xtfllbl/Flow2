#ifndef QJDFUNCATIONWIDGET_H
#define QJDFUNCATIONWIDGET_H

#include <QTreeWidget>
#include <xml/qjdreadxmlindex.h>
#include <qjdargu.h>
#include <QFile>

class QJDFuncationWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QJDFuncationWidget();

    QJDReadXMLIndex *readIndexXML;
    QJDArgu *argu;
    QFile fileXMLIndex;

private:
    QStringList allModuleName;
    QStringList allModuleGroup;
    QStringList allModulePath;
    QStringList allModuleDesc;
    QStringList allModuleType;


    void analysisIndexXML();

    void setWidgetData();


};

#endif // QJDFUNCATIONWIDGET_H

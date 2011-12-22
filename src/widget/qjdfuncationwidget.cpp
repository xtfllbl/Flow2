#include "qjdfuncationwidget.h"
#include <QDebug>

QJDFuncationWidget::QJDFuncationWidget()
{
    this->setHeaderHidden(true);
    this->setAnimated(true);

    readIndexXML=new QJDReadXMLIndex;
    argu=new QJDArgu();

    this->analysisIndexXML();
    this->setWidgetData();
}

void QJDFuncationWidget::analysisIndexXML()
{
    qDebug()<<" QJDFuncationWidget::analysisIndexXML()";
    fileXMLIndex.setFileName(argu->exePath+"/fun/index.xml");
    qDebug()<<fileXMLIndex.fileName();
    if(!fileXMLIndex.open(QFile::ReadOnly))
    {
        qDebug()<<"fileXMLIndex open filed";
    }
    readIndexXML->read(&fileXMLIndex);
}

void QJDFuncationWidget::setWidgetData()
{
    qDebug()<<"QJDFuncationWidget::setWidgetData()";
     allModuleName=readIndexXML->getModuleName();
     allModuleGroup=readIndexXML->getModuleGroup();
     allModulePath=readIndexXML->getModulePath();
     allModuleDesc=readIndexXML->getModuleDesc();
     allModuleType=readIndexXML->getModuleType();

     QTreeWidgetItem *ioItem = new QTreeWidgetItem;
     QTreeWidgetItem *processItem = new QTreeWidgetItem;
     QTreeWidgetItem *displayItem = new QTreeWidgetItem;
     ioItem->setText(0,"IO");
     processItem->setText(0,"Process");
     displayItem->setText(0,"Display");

     for(int i=0;i<allModuleName.count();i++)
     {
         if(allModuleGroup.at(i)=="IO")
         {
             QTreeWidgetItem *ioChildItem = new QTreeWidgetItem;
             ioChildItem->setText(0,allModuleName.at(i));
             ioChildItem->setToolTip(0,allModuleDesc.at(i));
             ioItem->addChild(ioChildItem);
         }
         if(allModuleGroup.at(i)=="Process")
         {
             QTreeWidgetItem *processChildItem = new QTreeWidgetItem;
             processChildItem->setText(0,allModuleName.at(i));
             processChildItem->setToolTip(0,allModuleDesc.at(i));
             processItem->addChild(processChildItem);
         }
         if(allModuleGroup.at(i)=="Display")
         {
             QTreeWidgetItem *displayChildItem = new QTreeWidgetItem;
             displayChildItem->setText(0,allModuleName.at(i));
             displayChildItem->setToolTip(0,allModuleDesc.at(i));
             displayItem->addChild(displayChildItem);
         }
     }

    this->addTopLevelItem(ioItem);
    this->addTopLevelItem(processItem);
    this->addTopLevelItem(displayItem);

    this->expandAll();
}

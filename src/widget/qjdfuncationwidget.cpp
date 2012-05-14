#include "qjdfuncationwidget.h"
#include <QDebug>

QJDFuncationWidget::QJDFuncationWidget()
{
    this->setIconSize(QSize(24,24));
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
    ioItem->setIcon(0,QIcon(":/src/images/io.png"));
    processItem->setText(0,"Process");
    processItem->setIcon(0,QIcon(":/src/images/process.png"));
    displayItem->setText(0,"Display");
    displayItem->setIcon(0,QIcon(":/src/images/display.png"));

    // tooltip设置为路径
    for(int i=0;i<allModuleName.count();i++)
    {
        if(allModuleGroup.at(i)=="IO")
        {
            QTreeWidgetItem *ioChildItem = new QTreeWidgetItem;
            ioChildItem->setText(0,allModuleName.at(i));
            ioChildItem->setToolTip(0,allModulePath.at(i));
            ioChildItem->setIcon(0,QIcon(":/src/images/io.png"));
            ioItem->addChild(ioChildItem);

        }
        if(allModuleGroup.at(i)=="Process")
        {
            QTreeWidgetItem *processChildItem = new QTreeWidgetItem;
            processChildItem->setText(0,allModuleName.at(i));
            processChildItem->setToolTip(0,allModulePath.at(i));
            processChildItem->setIcon(0,QIcon(":/src/images/process.png"));
            processItem->addChild(processChildItem);
        }
        if(allModuleGroup.at(i)=="Display")
        {
            QTreeWidgetItem *displayChildItem = new QTreeWidgetItem;
            displayChildItem->setText(0,allModuleName.at(i));
            displayChildItem->setToolTip(0,allModulePath.at(i));
            displayChildItem->setIcon(0,QIcon(":/src/images/display.png"));
            displayItem->addChild(displayChildItem);
        }
    }
    this->addTopLevelItem(ioItem);
    this->addTopLevelItem(processItem);
    this->addTopLevelItem(displayItem);

    this->expandAll();
}

void QJDFuncationWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"QJDFuncationWidget::mouseDoubleClickEvent";
    // 传出名称和路径
    qDebug()<<this->currentItem()->text(0)<<this->currentItem()->toolTip(0);
    emit sigFunDoubleClicked(currentItem()->text(0),currentItem()->toolTip(0));
}

#include "qjdprocesswidget.h"
#include <QLayout>
#include <QDebug>
#include <QAction>

QJDProcessWidget::QJDProcessWidget(QWidget *parent) :
    QWidget(parent)
{
    menuBar=new QMenuBar();
    tableWidget=new QTableWidget();

    menuBar->addMenu("File");

//    QAction *actAdd;
//    actAdd = new QAction(tr("&Add..."),this);
////    connect(actAdd, SIGNAL(triggered()), this, SLOT(addTask(QString)));

    menuBar->addAction(actAdd);
//    menuBar->addMenu("menu2");
//    menuBar->addMenu("menu3");

    QVBoxLayout *vLayout=new QVBoxLayout;
    vLayout->setMenuBar(menuBar);
    vLayout->addWidget(tableWidget);

    this->setLayout(vLayout);
}

void QJDProcessWidget::addTask(QString pid)
{
    tableWidget->setRowCount(6);
    tableWidget->setColumnCount(1);
    qDebug()<<"QJDProcessWidget::addTask"<<pid;
    QTableWidgetItem *item=new QTableWidgetItem;
    QString text=pid;
    item->setText(pid);
    tableWidget->setItem(0,0,item);

}

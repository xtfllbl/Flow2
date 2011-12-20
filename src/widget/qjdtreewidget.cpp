#include "qjdtreewidget.h"
#include <QDebug>
QJDTreeWidget::QJDTreeWidget()
{
    this->setAnimated(true);

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemPath(QTreeWidgetItem*)));
    this->setHeaderLabel("Folders:");
}

void QJDTreeWidget::itemPath(QTreeWidgetItem *item)
{
    QString path=item->toolTip(0);
    emit sigItemPath(path);
}

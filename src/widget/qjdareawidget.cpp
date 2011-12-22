#include "qjdareawidget.h"
#include <QDebug>
QJDAreaWidget::QJDAreaWidget()
{
    this->setAnimated(true);

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemPath(QTreeWidgetItem*)));
//    this->setHeaderLabel("Folders:");
    this->setHeaderHidden(true);
}

void QJDAreaWidget::itemPath(QTreeWidgetItem *item)
{
    QString path=item->toolTip(0);
    emit sigItemPath(path);
}

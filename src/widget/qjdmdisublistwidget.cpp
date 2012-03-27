#include "qjdmdisublistwidget.h"
#include <QMouseEvent>
#include <QDebug>

/// this widget will be added with flow
QJDMdiSubListWidget::QJDMdiSubListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void QJDMdiSubListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if(event->button()==Qt::MidButton)
    {
        // 这里connect了多次,导致后续错误
        connect(this,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClickedSlot(QListWidgetItem*)));
    }
}

void QJDMdiSubListWidget::itemClickedSlot(QListWidgetItem *item)
{
    qDebug()<<"emit sigMidButtonClicked";
    emit sigMidButtonClicked(item);
    // 及时断开链接
    disconnect(this,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClickedSlot(QListWidgetItem*)));
}

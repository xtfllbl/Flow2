#include "qjdmdisublistwidget.h"
#include <QMouseEvent>
#include <QDebug>

/// this widget will be added with flow
QJDMdiSubListWidget::QJDMdiSubListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    rightClickedItem=new QListWidgetItem;

    setSelectionMode(QAbstractItemView::ExtendedSelection);  // 多选条目

    menu=new QMenu;
    actDelFlow=new QAction("Delete Flow",this);
    actOpenCloseFlow=new QAction("Open/Close Flow",this);
//    actNewFlow=new QAction("new flow",this);
    menu->addAction(actDelFlow);
    menu->addAction(actOpenCloseFlow);
//    menu->addAction(actNewFlow);
    connect(actDelFlow,SIGNAL(triggered()),this,SLOT(delFlowSlot()));
    connect(actOpenCloseFlow,SIGNAL(triggered()),this,SLOT(openCloseFlowSlot()));
//    connect(actNewFlow,SIGNAL(triggered()),this,SLOT(actNewFlowClicked()));
}

void QJDMdiSubListWidget::contextMenuEvent(QContextMenuEvent *)
{
    /// 需要做完判断再显示
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void QJDMdiSubListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if(event->button()==Qt::MidButton)
    {
        qDebug()<<"mousePressEvent::mid";
        connect(this,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(midClickedSlot(QListWidgetItem*)));
    }
}

void QJDMdiSubListWidget::midClickedSlot(QListWidgetItem *item)
{
    qDebug()<<"emit sigMidButtonClicked";
    emit sigMidButtonClicked(item);
    // 及时断开链接
    disconnect(this,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(midClickedSlot(QListWidgetItem*)));
}

/// 删除flow按钮被按下,同时制作打开/关闭按钮
void QJDMdiSubListWidget::delFlowSlot()
{
    emit sigDelFlowClicked(selectedItems());  // 输出的是文件名而已
}

void QJDMdiSubListWidget::openCloseFlowSlot()
{
    emit sigOpenCloseFlowClicked(selectedItems());
}

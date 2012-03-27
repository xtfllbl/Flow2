#include "qjdareawidget.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QtTest/QTest>
QJDAreaWidget::QJDAreaWidget()
{
    this->setAnimated(true);

    absolutePath.clear();
//    this->setHeaderLabel("Folders:");
    this->setHeaderHidden(true);

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(returnPath(QTreeWidgetItem*,int)));

    // 右键菜单,需要连接到mainwindow里面去creat
    menu=new QMenu;
    actNewArea=new QAction("new area",this);
    actNewLine=new QAction("new line",this);
    actNewFlow=new QAction("new flow",this);
    menu->addAction(actNewArea);
    menu->addAction(actNewLine);
    menu->addAction(actNewFlow);
    connect(actNewArea,SIGNAL(triggered()),this,SLOT(actNewAreaClicked()));
    connect(actNewLine,SIGNAL(triggered()),this,SLOT(actNewLineClicked()));
    connect(actNewFlow,SIGNAL(triggered()),this,SLOT(actNewFlowClicked()));

    connect(this,SIGNAL(sigLevel(int)),this,SLOT(setContextMenu(int)));
    this->setContextMenuPolicy(Qt::DefaultContextMenu);

    // 由于这个信号,导致itemclicked右键接受不正常,被此处截取过去了,如何是好 !!!!!!!!!!!!!!!!!!!!!
    /// 难道显示了之后,选择了之后再判断?
    //    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
    //            this, SLOT(showContextMenu(const QPoint&)));
}

void QJDAreaWidget::actNewAreaClicked()
{
    // 连接到mainwindow
    emit sigActNewAreaClicked();
}

void QJDAreaWidget::actNewLineClicked()
{
    emit sigActNewLineClicked();
}

void QJDAreaWidget::actNewFlowClicked()
{
    emit sigActNewFlowClicked();
}
// 和custom那个一个性质...
void QJDAreaWidget::contextMenuEvent(QContextMenuEvent *)
{
    /// 需要做完判断再显示
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void QJDAreaWidget::showContextMenu(QPoint )
{
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void QJDAreaWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    qDebug()<<"mousePressEvent"<<event->button();
    if(event->button()==Qt::LeftButton)
    {
        qDebug()<<"left button clicked";
//        QTest::mouseClick(this,Qt::RightButton); //引入这个相当麻烦,不应当写入代码
    }
    if(event->button()==Qt::RightButton)
    {
        qDebug()<<"right button clicked";
//        QTest::mouseClick(this,Qt::RightButton); //引入这个相当麻烦,不应当写入代码
    }
}

/// 双击判断,判断是flow之后,如无创建过则创建新的sub mdi,
void QJDAreaWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"QJDAreaWidget::mouseDoubleClickEvent"<<event;
    // 必须为双击flow才做反映
    if(level()==3)
    {
        emit sigExistFlow();
    }
}

void QJDAreaWidget::returnPath(QTreeWidgetItem* item,int col)
{
    qDebug()<<"item Clicked --> returnPath";
    QString path=item->toolTip(0);
    emit sigItemPath(path);   // 这个path是不对的....


    //! item clicked 竟然不算右键单击的情况,很困扰,难道需要手工写一个?
   /// 需要返回当前item的路径
    absolutePath=item->toolTip(col);
    QString left;
    left=absolutePath.right(absolutePath.count()-homeDirPath.count()+1);
    qDebug()<<absolutePath
           <<homeDirPath
          <<left;

    // flow
    if(left.count(QRegExp("/"))==3)
    {
        setLevel(3);
    }
    // line
    if(left.count(QRegExp("/"))==2)
    {
        setLevel(2);
    }
    // area
    if(left.count(QRegExp("/"))==1)
    {
        setLevel(1);
    }
}

void QJDAreaWidget::setLevel(int level)
{
    _level=level;
    emit sigLevel(_level);
}

void QJDAreaWidget::setContextMenu(int lev)
{
    qDebug()<<"setContextMenu::"<<lev;  //仅发生在左键单击的时候
    if(lev==1)
    {
        actNewArea->setEnabled(true);
        actNewLine->setEnabled(true);
        actNewFlow->setEnabled(false);
    }
    if(lev==2)
    {
        actNewArea->setEnabled(false);
        actNewLine->setEnabled(true);
        actNewFlow->setEnabled(true);
    }
    if(lev==3)
    {
        actNewArea->setEnabled(false);
        actNewLine->setEnabled(false);
        actNewFlow->setEnabled(false);
    }
}

int QJDAreaWidget::level()
{
    return _level;
}

QString QJDAreaWidget::getAbsolutePath()
{
    return absolutePath;
}

void QJDAreaWidget::setHome(QString path)
{
    homeDirPath=path;
}

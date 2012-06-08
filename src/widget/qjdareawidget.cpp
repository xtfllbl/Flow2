#include "qjdareawidget.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QtTest/QTest>

QJDAreaWidget::QJDAreaWidget()
{
    this->setAnimated(true);
    this->setIconSize(QSize(24,24));

    absolutePath.clear();
//    this->setHeaderLabel("Folders:");
    this->setHeaderHidden(true);

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(returnPath(QTreeWidgetItem*,int)));

    // 右键菜单,需要连接到mainwindow里面去creat
    QJDMenuStyle *menuStyle=new QJDMenuStyle;
    menu=new QMenu;
    menu->setStyle(menuStyle);
    actNewArea=new QAction("Creat New Area",this);
    actNewArea->setIcon(QIcon(":/src/images/area.png"));

    actNewLine=new QAction("Creat New Line",this);
    actNewLine->setIcon(QIcon(":/src/images/line.png"));

    actNewFlow=new QAction("Creat New Flow",this);
    actNewFlow->setIcon(QIcon(":/src/images/flow.png"));

    actDelArea=new QAction("Delete Area",this);
    actDelArea->setIcon(QIcon(":/src/images/delete.png"));

    actDelLine=new QAction("Delete Line",this);
    actDelLine->setIcon(QIcon(":/src/images/delete.png"));

    actDelFlow=new QAction("Delete Flow",this);
    actDelFlow->setIcon(QIcon(":/src/images/delete.png"));

    actOpenFlow=new QAction("Open Flow",this);
    actOpenFlow->setIcon(QIcon(":/src/images/open.png"));

    actExcuteFlow=new QAction("Excute Flow",this);
    actExcuteFlow->setIcon(QIcon(":/src/images/excute.png"));

    menu->addAction(actOpenFlow);
    menu->addAction(actExcuteFlow);
    menu->addAction(actNewArea);
    menu->addAction(actNewLine);
    menu->addAction(actNewFlow);
    menu->addAction(actDelArea);
    menu->addAction(actDelLine);
    menu->addAction(actDelFlow);

    connect(actNewArea,SIGNAL(triggered()),this,SLOT(actNewAreaClicked()));
    connect(actNewLine,SIGNAL(triggered()),this,SLOT(actNewLineClicked()));
    connect(actNewFlow,SIGNAL(triggered()),this,SLOT(actNewFlowClicked()));
    connect(actDelArea,SIGNAL(triggered()),this,SLOT(actDelAreaClicked()));
    connect(actDelLine,SIGNAL(triggered()),this,SLOT(actDelLineClicked()));
    connect(actDelFlow,SIGNAL(triggered()),this,SLOT(actDelFlowClicked()));
    connect(actOpenFlow,SIGNAL(triggered()),this,SLOT(actOpenFlowClicked()));
    connect(actExcuteFlow,SIGNAL(triggered()),this,SLOT(actExcuteFlowClicked()));
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

void QJDAreaWidget::actDelAreaClicked()
{
    emit sigActDelAreaClicked();
}

void QJDAreaWidget::actDelLineClicked()
{
    emit sigActDelLineClicked();
}

void QJDAreaWidget::actDelFlowClicked()
{
    emit sigActDelFlowClicked();
}

void QJDAreaWidget::actOpenFlowClicked()
{
    emit sigActOpenFlowClicked();
}

void QJDAreaWidget::actExcuteFlowClicked()
{
    emit sigActExcuteFlowClicked();
}

// 右键菜单执行
void QJDAreaWidget::contextMenuEvent(QContextMenuEvent *)
{
    returnPath(currentItem(),currentColumn());
    /// 需要做完判断再显示
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
    QString path=item->toolTip(0);
    qDebug()<<"item Clicked --> returnPath"<<path;
    emit sigItemPath(path);


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
    // area
    if(lev==1)
    {
        actNewArea->setEnabled(true);
        actNewLine->setEnabled(true);
        actNewFlow->setEnabled(false);
        actNewArea->setVisible(true);
        actNewLine->setVisible(true);
        actNewFlow->setVisible(false);

        actDelArea->setEnabled(true);
        actDelLine->setEnabled(false);
        actDelFlow->setEnabled(false);
        actDelArea->setVisible(true);
        actDelLine->setVisible(false);
        actDelFlow->setVisible(false);

        actOpenFlow->setVisible(false);
        actExcuteFlow->setVisible(false);
    }
    // line
    if(lev==2)
    {
        actNewArea->setEnabled(false);
        actNewLine->setEnabled(false);
        actNewFlow->setEnabled(true);
        actNewArea->setVisible(false);
        actNewLine->setVisible(false);
        actNewFlow->setVisible(true);

        actDelArea->setEnabled(false);
        actDelLine->setEnabled(true);
        actDelFlow->setEnabled(false);
        actDelArea->setVisible(false);
        actDelLine->setVisible(true);
        actDelFlow->setVisible(false);

        actOpenFlow->setVisible(false);
        actExcuteFlow->setVisible(false);
    }
    // flow
    if(lev==3)
    {
        actNewArea->setEnabled(false);
        actNewLine->setEnabled(false);
        actNewFlow->setEnabled(false);
        actNewArea->setVisible(false);
        actNewLine->setVisible(false);
        actNewFlow->setVisible(false);

        actDelArea->setEnabled(false);
        actDelLine->setEnabled(false);
        actDelFlow->setEnabled(true);
        actDelArea->setVisible(false);
        actDelLine->setVisible(false);
        actDelFlow->setVisible(true);

        actOpenFlow->setVisible(true);
        actExcuteFlow->setVisible(true);
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

void QJDAreaWidget::removeCurrentLine_Flow()
{
    QTreeWidgetItem *cItem=currentItem();
    QTreeWidgetItem *pItem=cItem->parent();
    pItem->removeChild(cItem);
}

void QJDAreaWidget::removeCurrentArea()
{
    QTreeWidgetItem *tItem=currentItem();
    this->takeTopLevelItem(indexOfTopLevelItem(tItem));
}

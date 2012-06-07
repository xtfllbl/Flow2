#include "qjdprocesswidget.h"
#include <QLayout>
#include <QDebug>
#include <QAction>
#include <QDateTime>
#include <QHeaderView>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>

// TODO: 添加保存历史记录,新建的时候是需要读取历史记录的.
// 如何保存
QJDProcessWidget::QJDProcessWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Flow Viewer");
//    setWindowFlags(Qt::WindowStaysOnTopHint);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
//    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    move(0, desktop->height());
    setMinimumSize(650,200);

    menuBar=new QMenuBar();
    tableWidget=new QJDProcessTableWidget();

    menuBar->addMenu("File");
    tableWidget->setSortingEnabled(false);

    tableWidget->setColumnCount(6); // 否则设置抬头无效
    tableWidget->setRowCount(1);

    QTableWidgetItem *itemName=new QTableWidgetItem("Flow Name");
    QTableWidgetItem *itemStatus=new QTableWidgetItem("Status");
    QTableWidgetItem *itemStartTime=new QTableWidgetItem("Start Time");
    QTableWidgetItem *itemEndTime=new QTableWidgetItem("End Time");
    QTableWidgetItem *itemPID=new QTableWidgetItem("PID");
    QTableWidgetItem *itemLog=new QTableWidgetItem("Log");

    tableWidget->setHorizontalHeaderItem(0,itemName);
    tableWidget->setHorizontalHeaderItem(1,itemStatus);
    tableWidget->setHorizontalHeaderItem(2,itemPID);
    tableWidget->setHorizontalHeaderItem(3,itemStartTime);
    tableWidget->setHorizontalHeaderItem(4,itemEndTime);
    tableWidget->setHorizontalHeaderItem(5,itemLog);

    qDebug()<<tableWidget->horizontalHeaderItem(0);
    QVBoxLayout *vLayout=new QVBoxLayout;
    vLayout->setMenuBar(menuBar);
    vLayout->addWidget(tableWidget);

    this->setLayout(vLayout);

    /// 右键菜单,为什么没有效果
    tableWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  // 多选条目
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 选一行

    // 载入记录
    tableWidget->loadHistory();
}

void QJDProcessWidget::addTask(QString pid,QString name,QString log)
{
    qDebug()<<"QJDProcessWidget::addTask"<<pid<<name<<tableWidget->rowCount();

    int row=tableWidget->rowCount()-1;  //这是往后添加阿,不是往前添加
    QTableWidgetItem *itemName=new QTableWidgetItem;
    itemName->setText(name);
    tableWidget->setItem(row,0,itemName);

    QTableWidgetItem *itemPID=new QTableWidgetItem;
    QString textPID=pid;
    itemPID->setText(textPID);
    tableWidget->setItem(row,2,itemPID);

    QTableWidgetItem *itemStatus=new QTableWidgetItem;
    itemStatus->setText("Running");
    tableWidget->setItem(row,1,itemStatus);

    QTableWidgetItem *itemTime=new QTableWidgetItem;
    QDateTime ct;
    QString currentTime=ct.currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    itemTime->setText(currentTime);
    tableWidget->setItem(row,3,itemTime);

    QTableWidgetItem *itemEndTime=new QTableWidgetItem;
    QString endTime="Not Finished";
    itemEndTime->setText(endTime);
    tableWidget->setItem(row,4,itemEndTime);

    QTableWidgetItem *itemLog=new QTableWidgetItem;
    itemLog->setText(log);
    tableWidget->setItem(row,5,itemLog);

    tableWidget->resizeColumnsToContents();
    hashRow.insert(pid.toInt(),row);
    tableWidget->insertRow(tableWidget->rowCount());

    tableWidget->saveHistory();
    tableWidget->setCurrentItem(itemName);  //这就可以scroll了
//    tableWidget->scrollToBottom();
}

// 多进程时,需要依靠pid来修改行
void QJDProcessWidget::changeStatus(int pid, int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<"QJDProcessWidget::changeStatus::"<<exitCode<<exitStatus;
    // 0 normal ,1 crash
    int row=hashRow.value(pid);
    if(exitStatus==0)
    {
        QTableWidgetItem *itemStatus=new QTableWidgetItem;
        itemStatus->setText("Finished");
        tableWidget->setItem(row,1,itemStatus);

        QTableWidgetItem *itemTime=new QTableWidgetItem;
        QDateTime ct;
        QString currentTime=ct.currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        itemTime->setText(currentTime);
        tableWidget->setItem(row,4,itemTime);
    }
    if(exitStatus==1)
    {
        QTableWidgetItem *itemStatus=new QTableWidgetItem;
        itemStatus->setText("Crashed");
        tableWidget->setItem(row,1,itemStatus);

        QTableWidgetItem *itemTime=new QTableWidgetItem;
        QDateTime ct;
        QString currentTime=ct.currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        itemTime->setText(currentTime);
        tableWidget->setItem(row,4,itemTime);
    }

    tableWidget->resizeColumnsToContents();
    tableWidget->saveHistory();
    tableWidget->scrollToBottom();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QJDProcessTableWidget::QJDProcessTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    menu=new QMenu;

    actShowLog=new QAction("Show Log",this);
    menu->addAction(actShowLog);
    connect(actShowLog,SIGNAL(triggered()),this,SLOT(showLogSlot()));

    actDelLog=new QAction("Delete Record",this);
    menu->addAction(actDelLog);
    connect(actDelLog,SIGNAL(triggered()),this,SLOT(delSlot()));
}

void QJDProcessTableWidget::contextMenuEvent(QContextMenuEvent *)
{
    /// 这个如何解决呢?
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void QJDProcessTableWidget::showLogSlot()
{
    qDebug()<<"show log:";  //调用当前最后一列的数据,并打开
//    qDebug()<<"current row:"<<this->currentRow();
    QList<int> rows;
    for(int i=0;i<this->selectedItems().size();i++)
    {
        int j=this->selectedItems().at(i)->row();
        if(!rows.contains(j))
        {
            rows.append(j);
        }
    }

    for(int i=0;i<rows.size();i++)
    {
        QTableWidgetItem *item=this->item(rows.at(i),5);

        textBrowser=new QJDTextBrowser();
        if(textBrowser->isVisible()==false)
        {
            textBrowser->show();
        }
        QString fileName;
        fileName=item->text();
        textBrowser->openLog(fileName);
    }
}

void QJDProcessTableWidget::saveHistory()
{
//    qDebug()<<"saveHistory";
    // 保存所有记录
    QFile historyFile;
    QString historyFileName=QApplication::applicationDirPath()+"/opt/process_history.log";
    historyFile.setFileName(historyFileName);
    historyFile.open(QFile::WriteOnly);

    QTextStream ts(&historyFile);
    for(int i=0;i<this->rowCount()-1;i++)
    {
        for(int j=0;j<this->columnCount();j++)
        {
            QString itemStr=this->item(i,j)->text();
            ts<<itemStr<<"|";
        }
        ts<<"\n";
    }

    historyFile.close();
}

void QJDProcessTableWidget::loadHistory()
{
    /// 最好可以单独打开此窗口,然后显示已有记录
    // 读取并显示所有记录
    qDebug()<<"loadHistory";
    QFile historyFile;
    QString historyFileName=QApplication::applicationDirPath()+"/opt/process_history.log";
    historyFile.setFileName(historyFileName);
    historyFile.open(QFile::ReadOnly);
    QTextStream ts(&historyFile);
    QVector<QStringList> lineVector;
    // 隔行读取
    while(!ts.atEnd())
    {
        QString lineText=ts.readLine();
        QStringList lineTextList=lineText.split("|",QString::SkipEmptyParts);
        lineVector<<lineTextList;
//        qDebug()<<lineTextList;
    }

    this->setColumnCount(6);
    this->setRowCount(lineVector.size()+1);

    for(int i=0;i<lineVector.size();i++)
    {
        for(int j=0;j<lineVector[i].size();j++)
        {
            QTableWidgetItem *item=new QTableWidgetItem;
            QString text=lineVector[i].at(j);
            item->setText(text);
            this->setItem(i,j,item);
        }
    }
    historyFile.close();
}

void QJDProcessTableWidget::delSlot()
{
    // 执行多项删除
    QList<int> rows;
    for(int i=0;i<this->selectedItems().size();i++)
    {
        int j=this->selectedItems().at(i)->row();
        if(!rows.contains(j))
        {
            rows.append(j);
        }
    }

    /// 必须要先删除大的,再删除小的,否则大的行数不对了,需要先排序
    if(rows.size()>1)
    {
        int i,j,temp;
        for(j=0;j<rows.size()-1;j++)
        {
            for(i=0;i<rows.size()-1-j;i++)
            {
                if(rows[i]<rows[i+1])
                {
                    temp=rows[i];
                    rows[i]=rows[i+1];
                    rows[i+1]=temp;
                }
            }
        }
    }

    qDebug()<<"delLogSlot::"<<rows;
    for(int i=0;i<rows.size();i++)
    {
//        qDebug()<<"remove row:"<<rows.at(i);
        this->removeRow(rows.at(i));
    }
}

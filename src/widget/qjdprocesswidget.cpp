#include "qjdprocesswidget.h"
#include <QLayout>
#include <QDebug>
#include <QAction>
#include <QDateTime>
#include <QHeaderView>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include "signal.h"
#include "errno.h"

QJDProcessWidget::QJDProcessWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Job Viewer");
//    setWindowFlags(Qt::WindowStaysOnTopHint);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
//    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    move(0, desktop->height());
    setMinimumSize(650,200);

    menuBar=new QMenuBar();
    tableWidget=new QJDProcessTableWidget();
    tableWidget->setEditTriggers(0);
    // Menu "File"
    QMenu *fileMenu=new QMenu("File");
    QAction *actClearHistory=new QAction(this);
    actClearHistory->setText("Clear History");
    QAction *actClose=new QAction(this);
    actClose->setText("Close");
    fileMenu->addAction(actClearHistory);
    fileMenu->addAction(actClose);  

    menuBar->addMenu(fileMenu);
    connect(actClearHistory,SIGNAL(triggered()),tableWidget,SLOT(clearHistorySlot()));
    connect(actClose,SIGNAL(triggered()),this,SLOT(close()));

    // Menu "Action"
    QMenu *actionMenu=new QMenu("Action");
    QAction *actTer=new QAction(this);
    actTer->setText("Terminate");
    actTer->setIcon(QIcon(":/src/images/process-stop.png"));
    actionMenu->addAction(actTer);

    QAction *actCheck=new QAction(this);
    actCheck->setText("Manual Check Log Status");
    actionMenu->addAction(actCheck);

    menuBar->addMenu(actionMenu);
    connect(actTer,SIGNAL(triggered()),tableWidget,SLOT(terProcess()));
    connect(actCheck,SIGNAL(triggered()),tableWidget,SLOT(checkLogStatus()));

    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(7); // 否则设置抬头无效
    tableWidget->setRowCount(1);

    QTableWidgetItem *itemName=new QTableWidgetItem("Flow Name");
    QTableWidgetItem *itemStatus=new QTableWidgetItem("Process Status");
    QTableWidgetItem *itemLogStatus=new QTableWidgetItem("Log Status");
    QTableWidgetItem *itemStartTime=new QTableWidgetItem("Start Time");
    QTableWidgetItem *itemEndTime=new QTableWidgetItem("End Time");
    QTableWidgetItem *itemPID=new QTableWidgetItem("PID");
    QTableWidgetItem *itemLog=new QTableWidgetItem("Log");

    tableWidget->setHorizontalHeaderItem(0,itemName);
    tableWidget->setHorizontalHeaderItem(1,itemStatus);
    tableWidget->setHorizontalHeaderItem(2,itemLogStatus);
    tableWidget->setHorizontalHeaderItem(3,itemPID);
    tableWidget->setHorizontalHeaderItem(4,itemStartTime);
    tableWidget->setHorizontalHeaderItem(5,itemEndTime);
    tableWidget->setHorizontalHeaderItem(6,itemLog);

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
    tableWidget->setItem(row,3,itemPID);

    QTableWidgetItem *itemStatus=new QTableWidgetItem;
    itemStatus->setText("Running");
    tableWidget->setItem(row,1,itemStatus);

    QTableWidgetItem *itemLogStatus=new QTableWidgetItem;
    itemLogStatus->setText("");
    tableWidget->setItem(row,2,itemLogStatus);

    QTableWidgetItem *itemTime=new QTableWidgetItem;
    QDateTime ct;
    QString currentTime=ct.currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    itemTime->setText(currentTime);
    tableWidget->setItem(row,4,itemTime);

    QTableWidgetItem *itemEndTime=new QTableWidgetItem;
    QString endTime="Not Finished";
    itemEndTime->setText(endTime);
    tableWidget->setItem(row,5,itemEndTime);

    QTableWidgetItem *itemLog=new QTableWidgetItem;
    itemLog->setText(log);
    tableWidget->setItem(row,6,itemLog);

    tableWidget->resizeColumnsToContents();
    hashRow.insert(pid.toInt(),row);
    tableWidget->insertRow(tableWidget->rowCount());

    tableWidget->saveHistory();
    tableWidget->setCurrentItem(itemName);  //这就可以scroll了
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
        tableWidget->setItem(row,5,itemTime);
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
        tableWidget->setItem(row,5,itemTime);
    }

    QTableWidgetItem *itemLogStatus=new QTableWidgetItem;
    QTableWidgetItem *itemPath=tableWidget->item(row,6);
    QFile logFile(itemPath->text());
    if(!logFile.open(QFile::ReadOnly))
        qDebug()<<"log file open failed";
    QTextStream ts(&logFile);
    QString all=ts.readAll();
    if(all.contains("ERROR"))
    {
        itemLogStatus->setText("Error");
        tableWidget->setItem(row,2,itemLogStatus);
    }
    if(all.contains("complete"))
    {
        itemLogStatus->setText("Complete");
        tableWidget->setItem(row,2,itemLogStatus);
    }
    if(all.contains("terminate"))
    {
        itemLogStatus->setText("Terminated");
        tableWidget->setItem(row,2,itemLogStatus);
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

//    actContinue=new QAction(this);
//    actContinue->setText("Continue");
//    actContinue->setIcon(QIcon(":/src/images/excute.png"));

//    actStop=new QAction(this);
//    actStop->setText("Stop");
//    actStop->setIcon(QIcon(":/src/images/norun.png"));

//    actPause=new QAction(this);
//    actPause->setText("Pause");
//    actPause->setIcon(QIcon(":/src/images/media-playback-stop.png"));

//    actKill=new QAction(this);
//    actKill->setText("Kill");
//    actKill->setIcon(QIcon(":/src/images/gtk-no.png"));

    actTer=new QAction(this);
    actTer->setText("Terminate");
    actTer->setIcon(QIcon(":/src/images/process-stop.png"));

//    menu->addAction(actContinue);
//    menu->addAction(actStop);
//    menu->addAction(actPause);
//    menu->addAction(actKill);
    menu->addAction(actTer);

//    connect(actContinue,SIGNAL(triggered()),this,SLOT(conProcess()));
//    connect(actStop,SIGNAL(triggered()),this,SLOT(stopProcess()));
//    connect(actPause,SIGNAL(triggered()),this,SLOT(hanProcess()));
//    connect(actKill,SIGNAL(triggered()),this,SLOT(killProcess()));
    connect(actTer,SIGNAL(triggered()),this,SLOT(terProcess()));
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
        QTableWidgetItem *item=this->item(rows.at(i),6);

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
        if(lineText=="")
        {
            return;
        }
        QStringList lineTextList=lineText.split("|"/*,QString::SkipEmptyParts*/);
        lineVector<<lineTextList;
//        qDebug()<<lineTextList;
    }

    this->setColumnCount(7);
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

    this->saveHistory();
}

void QJDProcessTableWidget::clearHistorySlot()
{
    int ret=QMessageBox::warning(this,"Warning!","Do you really want to clear ALL the history?",
                                 QMessageBox::Ok|QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Ok:
        qDebug()<<"start remove"<<rowCount();
        for(int i=0;i<this->rowCount()-1;i++)
        {
            QFile file(this->item(i,6)->text());
            file.remove();
        }
        this->setRowCount(0);
        this->setRowCount(1);
        this->saveHistory();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }

}

//void QJDProcessTableWidget::killProcess()
//{
//    send_to_selected(SIGKILL);
//}

void QJDProcessTableWidget::terProcess()
{
    send_to_selected(SIGTERM);
}

///// 虽说是暂停,但一般就关闭了
//void QJDProcessTableWidget::hanProcess()
//{
//    send_to_selected(SIGHUP);
//}
//void QJDProcessTableWidget::stopProcess()
//{
//    send_to_selected(SIGSTOP);
//}
//void QJDProcessTableWidget::conProcess()
//{
//    send_to_selected(SIGCONT);
//}

// 需要当前的pid,还需要能多选处理...
void QJDProcessTableWidget::send_to_selected(int sig)
{
    // 选中行
    QList<int> rows;
    for(int i=0;i<this->selectedItems().size();i++)
    {
        int j=this->selectedItems().at(i)->row();
        if(!rows.contains(j))
        {
            rows.append(j);
        }
    }

    // 取得pid
    QList<int> processID_List;
    for(int i=0;i<rows.size();i++)
    {
        // 这里没有hashRow了
        processID_List.append(   this->item(rows.at(i),3)->text().toInt()   );
    }

    // 完成信号传输
    for(int i=0;i<processID_List.size();i++)
    {
        sendsig(processID_List.at(i), sig);
    }
}

void QJDProcessTableWidget::sendsig(int pid, int sig)
{
    if(kill(pid, sig) < 0)
    {
        if(errno == EPERM)
        {
            QString s;
            s.sprintf("You do not have permission to send a signal to"
                      " process %d (", pid);
            s.append(").\n"
                     "Only the super-user and the owner of the process"
                     " may send signals to it.");
            QMessageBox::warning(this, "Permission denied", s);
        }
    }
}

void QJDProcessTableWidget::checkLogStatus()
{
    for(int i=0;i<this->rowCount()-1;i++)
    {
        QTableWidgetItem *itemLogStatus=new QTableWidgetItem;
        QTableWidgetItem *itemPath=this->item(i,6);
        QFile logFile(itemPath->text());
        if(!logFile.open(QFile::ReadOnly))
            qDebug()<<"log file open failed";
        QTextStream ts(&logFile);
        QString all=ts.readAll();
        if(all.contains("ERROR"))
        {
            itemLogStatus->setText("Error");
            this->setItem(i,2,itemLogStatus);
        }
        if(all.contains("complete"))
        {
            itemLogStatus->setText("Complete");
            this->setItem(i,2,itemLogStatus);
        }
        if(all.contains("terminate"))
        {
            itemLogStatus->setText("Terminated");
            this->setItem(i,2,itemLogStatus);
        }
    }
    this->resizeColumnsToContents();
    this->saveHistory();
}


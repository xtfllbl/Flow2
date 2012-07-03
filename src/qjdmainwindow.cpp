#include "qjdmainwindow.h"
#include "ui_qjdmainwindow.h"
#include <QDebug>
#include <QLayout>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <stdlib.h>

#include "widget/qjdnewarea.h"
#include "widget/qjdnewline.h"
#include "widget/qjdnewflow.h"
// not necessary
#include "widget/qjdmdisublistwidget.h"

// 是否需要改造treeview,是,所有界面需要手动书写
QJDMainWindow::QJDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QJDMainWindow)
{
    ui->setupUi(this);

    argu=new QJDArgu;
    // 获取环境变量中数据目录地址

    if(argu->JD_FileType!="")
    {
        QStringList fileTypeList=argu->JD_FileType.split(",");
        settings.setValue("FileType",fileTypeList);
    }
    qDebug()<<"File typelist::"<<settings.value("FileType").toStringList();
    if(argu->JD_FlowDataDir!="")
    {
        _HOME_DIR=argu->JD_FlowDataDir;
    }
    else
    {
        _HOME_DIR=settings.value("HOME").toString();
    }
    settings.setValue("HOME",_HOME_DIR);


    canCreatNewFlow=false;

    pathLabel1=new QJDLabel;
    pathLabel1->setText("Address::");
    pathLabel1->setMaximumHeight(10);

    pathLabel2=new QJDLabel;
    pathLabel2->setFrameShape(QFrame::Box);
    pathLabel2->setFrameShadow(QFrame::Raised);
    pathLabel2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    pathLabel2->setMaximumHeight(30);

    areaHeadWidget=new QJDAreaHeadWidget;
    areaWidget=new QJDAreaWidget();
    areaWidget->setColumnCount(1);
    connect(areaWidget,SIGNAL(sigLevel(int)),this,SLOT(enableNew(int)));

    propertyWidget=new QJDPropertyWidget;
    mdiWidget=new QJDMdi; // connect with areawidget & funcationwidget

    tabWidget=new QJDTabWidget();
    tabWidget->setIconSize(QSize(24,24));
//    tabWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    QWidget *tab1=new QWidget;
    QGridLayout *gLayout1=new QGridLayout(tab1);
    gLayout1->addWidget(propertyWidget);
    tabWidget->addTab(tab1,QIcon(":/src/images/propertyviewer"),"Property Viewer");
    QWidget *tab2=new QWidget;
    QGridLayout *gLayout2=new QGridLayout(tab2);
    gLayout2->addWidget(mdiWidget);
    tabWidget->addTab(tab2,QIcon(":/src/images/flowviewer"), "Flow Editor");
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(setWidgetVisible(int)));

    funcationHeadWidget=new QJDFuncationHeadWidget;
    funcationWidget=new QJDFuncationWidget();

    areaSplitterWidget=new QWidget;
    funcationSplitterWidget=new QWidget;
    QVBoxLayout *areaLayout=new QVBoxLayout(areaSplitterWidget);
    QVBoxLayout *funcationLayout=new QVBoxLayout(funcationSplitterWidget);
    areaLayout->addWidget(areaHeadWidget);
    areaLayout->addWidget(areaWidget);
    funcationLayout->addWidget(funcationHeadWidget);
    funcationLayout->addWidget(funcationWidget);

    splitter=new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(areaSplitterWidget);
    splitter->addWidget(tabWidget);
    splitter->addWidget(funcationSplitterWidget);

//    areaSplitterWidget->show();
//    tabWidget->show();
    funcationSplitterWidget->hide();

    areaWidget->setHome(_HOME_DIR);
    // area 必须先connect
    connect(areaWidget,SIGNAL(sigItemPath(QString)),pathLabel2,SLOT(setText(QString)));
    connect(areaWidget,SIGNAL(sigItemPath(QString)),propertyWidget,SLOT(setPropertyData(QString)));
    setHomeDir(_HOME_DIR);
    areaWidget->expandToDepth(1);

    QHBoxLayout *pathLayout=new QHBoxLayout;
    pathLayout->addWidget(pathLabel1);
    pathLayout->addWidget(pathLabel2);

//    QHBoxLayout *hLayout=new QHBoxLayout;
//    hLayout->addWidget(areaWidget);
//    hLayout->addWidget(tabWidget);
//    hLayout->addWidget(functionWidget);

    QVBoxLayout *vLayout=new QVBoxLayout;
    vLayout->addLayout(pathLayout);
    vLayout->addWidget(splitter);

    ui->centralWidget->setLayout(vLayout);

    // area
    connect(areaWidget,SIGNAL(sigActNewAreaClicked()),this,SLOT(on_actionNewArea_triggered()));
    connect(areaWidget,SIGNAL(sigActNewLineClicked()),this,SLOT(on_actionNewLine_triggered()));
    connect(areaWidget,SIGNAL(sigActNewFlowClicked()),this,SLOT(on_actionNewFlow_triggered()));
    connect(areaWidget,SIGNAL(sigActDelAreaClicked()),this,SLOT(on_actionDelArea_triggered()));
    connect(areaWidget,SIGNAL(sigActDelLineClicked()),this,SLOT(on_actionDelLine_triggered()));
    connect(areaWidget,SIGNAL(sigActDelFlowClicked()),this,SLOT(on_actionDelFlow_triggered()));
    connect(areaWidget,SIGNAL(sigActOpenFlowClicked()),this,SLOT(openFlowSlot()));
    connect(areaWidget,SIGNAL(sigActExcuteFlowClicked()),this,SLOT(excuteFlowSlot()));

    connect(areaWidget,SIGNAL(sigExistFlow()),this,SLOT(showExistFlow()));

    // funcation
    connect(funcationWidget,SIGNAL(sigFunDoubleClicked(QString,QString)),mdiWidget,SLOT(addFuncation(QString,QString)));
//    connect(funcationWidget,SIGNAL(sigFunDoubleClicked(QString,QString)),this,SLOT(copyFlowXML()));

    // head
    connect(areaHeadWidget,SIGNAL(sigRefreshClicked()),this,SLOT(refreshList()));
    connect(areaHeadWidget,SIGNAL(sigExpandClicked(int)),areaWidget,SLOT(expandToDepth(int)));
    connect(areaHeadWidget,SIGNAL(sigCollapseClicked()),areaWidget,SLOT(collapseAll()));
    connect(funcationHeadWidget,SIGNAL(sigExpandClicked(int)),funcationWidget,SLOT(expandToDepth(int)));
    connect(funcationHeadWidget,SIGNAL(sigCollapseClicked()),funcationWidget,SLOT(collapseAll()));

    /// mdi
//    connect(areaWidget,SIGNAL(),mdiWidget,SLOT(newSubWindow()));
//    connect(funcationWidget,SIGNAL(),mdiWidget,SLOT(addFlow()));

    // 必须添加流程后才能点击
    ui->actionExcuteFlow->setEnabled(false);
    connect(mdiWidget,SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this,SLOT(excuteDecide(QMdiSubWindow*)));
    showMaximized();
}

QJDMainWindow::~QJDMainWindow()
{
    delete ui;
}

void QJDMainWindow::refreshList()
{
    setHomeDir(getHomeDir());
    areaWidget->expandToDepth(1);
}
/// TODO:名称和路径需要联系起来
// 不能使用QHash ,会出现string相同的情况,那用什么方法呢
// QMultiMap??
void QJDMainWindow::setHomeDir(QString homePath)
{
    areaWidget->clear();

    /// 第一层 -- 工区
    QDir dir1;

    // 这个需要能设置,程序需要有settings.ini
    dir1.setPath(homePath);
    QStringList dirLev1;
    dirLev1<<dir1.entryList(QDir::NoDotAndDotDot|QDir::Dirs);
//    qDebug()<<dir1.count(); // 包含./..

    /// 第二层 -- 线, 目前要向里面加入data文件夹,data文件夹与flow并列并且继续有往下的选择,可以不用descname
    QStringList areaStringList;
    QStringList areaPathList;
    QStringList lineStringList;
    QStringList linePathList;

    QVector<QStringList> flowStringList;
    QVector<QStringList> flowPathList;

    QVector<QVector<QStringList> > dataStringList;
    QVector<QVector<QStringList> >  dataPathList;
    for(int i=0;i<dirLev1.count();i++)
    {
        // 遍历
        QDir dir2;
        QString dir2path=dir1.path()+"/"+dirLev1.at(i);
        dir2.setPath(dir2path);
        QStringList dirLev2;
        dirLev2=dir2.entryList(QDir::NoDotAndDotDot|QDir::Dirs);
        areaPathList<<dir2path;

        // 解析 DescName -- 工区名称
        QFile file2;
        file2.setFileName(dir2path+"/DescName");
        file2.open(QFile::ReadOnly);
        QTextStream stream2(&file2);
        QString areatmp=stream2.readAll();
        areatmp.chop(1);
        areaStringList<<areatmp;   // 路径就是dir2path
//        qDebug()<<dir2path;
        file2.close();

        /// 第三层 -- 流程/Data, 同一层的data文件夹需要特殊处理
        for(int j=0;j<dirLev2.count();j++)
        {
            QDir dir3;
            QString dir3path=dir2.path()+"/"+dirLev2.at(j);
            dir3.setPath(dir3path);
            QStringList dirLev3;
            dirLev3=dir3.entryList(QDir::NoDotAndDotDot|QDir::Dirs);  // 线名
            linePathList<<dir3path;

            // 解析 DescName -- 线名称
            QFile file3;
            file3.setFileName(dir3path+"/DescName");
            file3.open(QFile::ReadOnly);
            QTextStream stream3(&file3);
            QString linetmp=stream3.readAll();
            linetmp.chop(1);
            lineStringList<<linetmp;
            file3.close();
//            qDebug()<<"line::"<<lineStringList;

            /// 第四层 -- 具体流程
            flowStringList.resize(dirLev2.count());
            flowPathList.resize(dirLev2.count());
            dataStringList.resize(dirLev2.count());
            dataPathList.resize(dirLev2.count());
            for(int k=0;k<dirLev3.count();k++)
            {
                // 应当没有文件夹了,只剩下文件了
                QDir dir4;
                QString dir4path=dir3.path()+"/"+dirLev3.at(k);
                dir4.setPath(dir4path);
                QStringList dirLev4;
                dirLev4=dir4.entryList(QDir::NoDotAndDotDot|QDir::Files);  // 文件名列表了
                flowPathList[j]<<dir4path;
                /// 底下应当有个记录流程xml文件

                // 解析 DescName -- 线名称
                QFile file4;
                file4.setFileName(dir4path+"/DescName");
                file4.open(QFile::ReadOnly);
                QTextStream stream4(&file4);
                QString flowtmp=stream4.readAll();
                flowtmp.chop(1);
                flowStringList[j]<<flowtmp;  // 只有在流程里才会有,其他的文件夹内不会有这个
                file4.close();
//                qDebug()<<"flow::"<<flowStringList;

                /// 第五层 -- 数据存放,不应当再像前面一样完全扫描了.需要列出文件类型目录即可
                //! !! 如何准确放到某条线下面去,每条线都有,目前还是没有解决这个问题
                // 也就是说需要二维数组来确定
                if(flowtmp=="Data")
                {
                    dataStringList[j].resize(dirLev3.count());
                    dataPathList[j].resize(dirLev3.count());
                    for(int l=0;l<dirLev4.count();l++)
                    {
                        // 应当没有文件夹了,只剩下文件了
                        QDir dir5;
                        dir5.setPath(dir4.path());
                        QStringList dirLev5;
                        dirLev5=dir5.entryList(QDir::NoDotAndDotDot|QDir::AllDirs);  // 文件名列表了
//                        qDebug()<<dir4.path()<<"dirLev5::"<<dirLev5;  // 怎么是空的??
                        dataStringList[j][k].append(dirLev5);
                        /// 底下应当有个记录流程xml文件

                        for(int m=0;m<dirLev5.size();m++)
                        {
                            QString dataPath=dir4.path()+"/"+dirLev5.at(m);
                            dataPathList[j][k].append(dataPath);
                        }
                    }
                    // dataString为空 path为data descName的path
//                    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!"<<dirLev4.count()<<dataStringList;
                }
            }
        }
//        qDebug()<<"\nstart set:::"<<areaStringList.at(i)<<lineStringList<<flowStringList;
//        qDebug()<<"Data:::"<<dataStringList;

        setAreaWidget(areaStringList.at(i),areaPathList.at(i),lineStringList,
                      linePathList,flowStringList,flowPathList,dataStringList,dataPathList);

        /// 清理
        flowStringList.clear();
        flowPathList.clear();
        lineStringList.clear();
        linePathList.clear();
        dataStringList.clear();
        dataPathList.clear();
    }

    if(!areaStringList.isEmpty())
    {
//        qDebug()<<areaWidget->topLevelItem(0)->text(0);
        areaWidget->setCurrentItem(areaWidget->topLevelItem(0));
        areaWidget->returnPath(areaWidget->currentItem(),areaWidget->currentColumn());
    }
}

void QJDMainWindow::setAreaWidget(QString areaString, QString areaPath, QStringList lineStringList,
                                  QStringList linePathList, QVector<QStringList> flowStringList,
                                  QVector<QStringList> flowPathList,QVector<QVector<QStringList> > dataStringList,
                                  QVector<QVector<QStringList> > dataPathList)
{
//    qDebug()<<"setAreaWidget::"<<flowStringList<<flowPathList;
    QTreeWidgetItem *areaItem = new QTreeWidgetItem;
    areaItem->setText(0,areaString);
    areaItem->setToolTip(0,areaPath);
    areaItem->setIcon(0,QIcon(":/src/images/area.png"));

    for(int j=0;j<lineStringList.count();j++)
    {
        QTreeWidgetItem *lineItem = new QTreeWidgetItem;
        lineItem->setText(0,lineStringList.at(j));
        lineItem->setToolTip(0,linePathList.at(j));
        lineItem->setIcon(0,QIcon(":/src/images/line.png"));

        for(int k=0;k<flowStringList[j].size();k++)
        {
            QTreeWidgetItem *flowItem = new QTreeWidgetItem;
            flowItem->setText(0,flowStringList[j].at(k));
            flowItem->setToolTip(0,flowPathList[j].at(k));
            if(flowStringList[j].at(k)=="Data")
            {
                flowItem->setIcon(0,QIcon(":/src/images/data.png"));
            }
            else
            {
                flowItem->setIcon(0,QIcon(":/src/images/flow.png"));
            }
            lineItem->addChild(flowItem);

            // Scan/Pick
            for(int l=0;l<dataStringList[j][k].size();l++)
            {
                QTreeWidgetItem *dataItem = new QTreeWidgetItem;
                dataItem->setText(0,dataStringList[j][k].at(l));
                dataItem->setToolTip(0,dataPathList[j][k].at(l));
//                dataItem->setIcon(0,QIcon(":/src/images/flow.png"));
                flowItem->addChild(dataItem);
//                qDebug()<<lineStringList.count()<<j<<flowStringList[j].size()<<k<<dataStringList[k].size()<<l;
            }
        }

        areaItem->addChild(lineItem);
    }
    areaWidget->addTopLevelItem(areaItem);
}

void QJDMainWindow::on_actionExit_triggered()
{
    exit(1);
}

void QJDMainWindow::closeEvent(QCloseEvent *)
{
    exit(1);
}

void QJDMainWindow::setWidgetVisible(int tabIndex)
{
    if(tabIndex==0)  // Property
    {
        funcationSplitterWidget->hide();
    }
    if(tabIndex==1)  // Flow
    {
        funcationSplitterWidget->show();
    }
}

void QJDMainWindow::enableNew(int level)
{
    if(level==1)
    {
        ui->actionNewArea->setEnabled(true);
        ui->actionNewLine->setEnabled(true);
        ui->actionNewFlow->setEnabled(false);
        ui->actionNewArea->setVisible(true);
        ui->actionNewLine->setVisible(true);
        ui->actionNewFlow->setVisible(false);

        ui->actionDelArea->setEnabled(true);
        ui->actionDelLine->setEnabled(false);
        ui->actionDelFlow->setEnabled(false);
        ui->actionDelArea->setVisible(true);
        ui->actionDelLine->setVisible(false);
        ui->actionDelFlow->setVisible(false);
    }
    if(level==2)
    {
        ui->actionNewArea->setEnabled(false);
        ui->actionNewLine->setEnabled(false);
        ui->actionNewFlow->setEnabled(true);
        ui->actionNewArea->setVisible(false);
        ui->actionNewLine->setVisible(false);
        ui->actionNewFlow->setVisible(true);

        ui->actionDelArea->setEnabled(false);
        ui->actionDelLine->setEnabled(true);
        ui->actionDelFlow->setEnabled(false);
        ui->actionDelArea->setVisible(false);
        ui->actionDelLine->setVisible(true);
        ui->actionDelFlow->setVisible(false);
    }
    if(level==3)
    {
        ui->actionNewArea->setEnabled(false);
        ui->actionNewLine->setEnabled(false);
        ui->actionNewFlow->setEnabled(false);
        ui->actionNewArea->setVisible(false);
        ui->actionNewLine->setVisible(false);
        ui->actionNewFlow->setVisible(false);

        ui->actionDelArea->setEnabled(false);
        ui->actionDelLine->setEnabled(false);
        ui->actionDelFlow->setEnabled(true);
        ui->actionDelArea->setVisible(false);
        ui->actionDelLine->setVisible(false);
        ui->actionDelFlow->setVisible(true);
    }
}

/// 除了要有新增的还需要能删除的,右键点击之后也是需要做这个
void QJDMainWindow::on_actionNewArea_triggered()
{
    QJDNewArea *newArea=new QJDNewArea;
    qDebug()<<"New Area"<<newArea;  // it will have different new area
    connect(newArea,SIGNAL(sigAreaName(QString)),this,SLOT(creatNewArea(QString)));
    newArea->exec();
}

void QJDMainWindow::on_actionNewLine_triggered()
{
    QJDNewLine *newLine=new QJDNewLine;
    qDebug()<<"New Line"<<newLine;  // it will have different new area
    connect(newLine,SIGNAL(sigLineName(QString)),this,SLOT(creatNewLine(QString)));
    newLine->exec();

}

// 除了创建窗口输入名称之外,输入完毕之后还需要创建一个submdi
void QJDMainWindow::on_actionNewFlow_triggered()
{
    QJDNewFlow *newFlow=new QJDNewFlow;
    qDebug()<<"New Flow"<<newFlow;  // it will have different new area
    /// 万一重复了呢?不需要执行以下的语句了嗄
    connect(newFlow,SIGNAL(sigFlowName(QString)),this,SLOT(creatNewFlow(QString)));
    newFlow->exec();
    // ---------------------------------------------------------------------- //
    // 模态,以下语句将在已返回的情况下执行,除了需要名称之外,路径也需要
    qDebug()<<"can?::"<<canCreatNewFlow
           <<"|after new flow->exec!"<<newFlow->getFlowName()<<areaWidget->returnPathStr;
    qDebug()<<"level::"<<areaWidget->level();
    if(canCreatNewFlow==true)
    {
        if(areaWidget->level()==2)
        {
            QString linePath=areaWidget->getAbsolutePath();
            mdiWidget->newSubWindow(newFlow->getFlowName(),linePath);
        }

        tabWidget->setCurrentIndex(1);
        setHomeDir(getHomeDir());
        areaWidget->expandToDepth(1);
    }
}

void QJDMainWindow::on_actionDelArea_triggered()
{
    /// 需要添加确认选项
    QMessageBox msgBox;
     msgBox.setText("This area will be deleted.");
     msgBox.setInformativeText("Do you really want to delete this area?");
     msgBox.setIcon(QMessageBox::Question);
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
     msgBox.setDefaultButton(QMessageBox::No);
     int ret = msgBox.exec();

     QFileInfo f(areaWidget->getAbsolutePath());
     switch (ret) {
       case QMessageBox::Yes:
         // delete
         deleteDir(f);
         break;
       case QMessageBox::No:
           // Don't delete
         return;
           break;
       default:
           // should never be reached
           break;
     }

     if(mdiWidget->subWindowList().size()!=0)
     {
         QString title=areaWidget->currentItem()->text(0);
         QList<QString> valueList=mdiWidget->hashSubMdiName.values();

         for(int i=0;i<valueList.size();i++)
         {
             QStringList trueValueList=valueList.at(i).split("::");  // 彻底防止误删
             if(trueValueList.at(0).contains(title,Qt::CaseInsensitive))
             {
                 mdiWidget->removeSubWindow(mdiWidget->hashSubMdiName.key(valueList.at(i)));
             }
         }
     }

     areaWidget->removeCurrentArea();
}

void QJDMainWindow::on_actionDelLine_triggered()
{
    QMessageBox msgBox;
     msgBox.setText("This line will be deleted.");
     msgBox.setInformativeText("Do you really want to delete this line?");
     msgBox.setIcon(QMessageBox::Question);
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
     msgBox.setDefaultButton(QMessageBox::No);
     int ret = msgBox.exec();
     QFileInfo f(areaWidget->getAbsolutePath());

     switch (ret) {
       case QMessageBox::Yes:
         // delete
         deleteDir(f);
         break;
       case QMessageBox::No:
           // Don't delete
         return;
           break;
       default:
           // should never be reached
           break;
     }

     /// 需要删掉线下的所有窗口,如何找出来呢???
     QString title=areaWidget->currentItem()->parent()->text(0)+"::"+
             areaWidget->currentItem()->text(0);
     QList<QString> valueList=mdiWidget->hashSubMdiName.values();
//     qDebug()<<"???????"<<title<<valueList;  // 无问题
     for(int i=0;i<valueList.size();i++)
     {
         if(valueList.at(i).contains(title,Qt::CaseInsensitive))
         {
             mdiWidget->removeSubWindow(mdiWidget->hashSubMdiName.key(valueList.at(i)));
         }
     }

     areaWidget->removeCurrentLine_Flow();
}

void QJDMainWindow::on_actionDelFlow_triggered()
{
    QMessageBox msgBox;
     msgBox.setText("This flow will be deleted.");
     msgBox.setInformativeText("Do you really want to delete this flow?");
     msgBox.setIcon(QMessageBox::Question);
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
     msgBox.setDefaultButton(QMessageBox::No);
     int ret = msgBox.exec();
     QFileInfo f(areaWidget->getAbsolutePath());

     switch (ret) {
       case QMessageBox::Yes:
         // delete
         deleteDir(f);
         break;
       case QMessageBox::No:
           // Don't delete
         return;
           break;
       default:
           // should never be reached
           break;
     }

     // 这个流程如果已经打开的话,也需要在界面处进行关闭
//     mdiWidget->removeSubWindow();

     QString title=areaWidget->currentItem()->parent()->parent()->text(0)+"::"+
             areaWidget->currentItem()->parent()->text(0)+"::"+areaWidget->currentItem()->text(0);
//     qDebug()<<title;
     mdiWidget->removeSubWindow(mdiWidget->hashSubMdiName.key(title));

     areaWidget->removeCurrentLine_Flow();
}


void QJDMainWindow::showExistFlow()
{
    // 切换到flow
    tabWidget->setCurrentIndex(1);

    QString flowPath=areaWidget->getAbsolutePath();
    QString flowName=areaWidget->currentItem()->text(0);
    mdiWidget->showExistSubWindow(flowName,flowPath);

}

void QJDMainWindow::on_actionSetDataHome_triggered()
{
    qDebug()<<"setDataHome"; // 此处是否要判断目录是否正确??
    QString homePath = QFileDialog::getExistingDirectory(this, tr("Choose Home Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(homePath!="")
    {
        setHomeDir(homePath);  // it works
        settings.setValue("HOME",homePath);
        _HOME_DIR=homePath;
        areaWidget->setHome(_HOME_DIR);
        areaWidget->expandToDepth(1);
    }
    else
    {
        qDebug()<<"set data home failed";
    }
}

QString QJDMainWindow::getHomeDir()
{
    return _HOME_DIR;
}

void QJDMainWindow::creatNewArea(QString areaName)
{
    QString lowerCaseAreaName;
    lowerCaseAreaName=areaName.toLower();
    lowerCaseAreaName.simplified();
    lowerCaseAreaName.remove(" ");
    qDebug()<<"creatNewArea::"<<areaName<<lowerCaseAreaName;  // 所有大写阿,带空格的之类的统一小写,并且去空格

    //1. 创建文件夹
    QDir newDir;
    newDir.setPath(getHomeDir());
    if(newDir.exists(lowerCaseAreaName))
    {
        QMessageBox::warning(this,"Warning!","Do not creat the same AREA again!");
        return;
    }

    if(!newDir.mkdir(lowerCaseAreaName))
        qDebug()<<"Creat New Area Dir failed";

    //2. 创建.Desc
    QFile newDesc;
    newDesc.setFileName(newDir.path()+"/"+lowerCaseAreaName+"/DescName");
    if(!newDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat new desc open failed";
    }
    QTextStream ts(&newDesc);
    ts<<areaName<<"\n";
    newDesc.close();

    setHomeDir(getHomeDir());
    areaWidget->expandToDepth(1);
}

void QJDMainWindow::creatNewLine(QString lineName)
{
    QString lowerCaseLineName;
    lowerCaseLineName=lineName.toLower();
    lowerCaseLineName.simplified();
    lowerCaseLineName.remove(" ");
    qDebug()<<"creatNewLine::"<<lineName<<lowerCaseLineName;  // 所有大写阿,带空格的之类的统一小写,并且去空格

    //1. 创建文件夹,需要获取当前area文件夹名称,通过当前的名称获取
    QDir newDir;
    newDir.setPath(areaWidget->getAbsolutePath());
    if(newDir.exists(lowerCaseLineName))
    {
        QMessageBox::warning(this,"Warning!","Do not creat the same LINE again!");
        return;
    }

    if(!newDir.mkdir(lowerCaseLineName))
        qDebug()<<"Creat New Line Dir failed";

    //2. 创建.Line Desc
    QFile newDesc;
    newDesc.setFileName(newDir.path()+"/"+lowerCaseLineName+"/DescName");
    if(!newDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat new desc open failed";
    }
    QTextStream ts(&newDesc);
    ts<<lineName<<"\n";
    newDesc.close();

    // 3. 创建Data文件夹
    QDir dataDir;
    QString dataDirPath=areaWidget->getAbsolutePath()+"/"+lowerCaseLineName;
    dataDir.setPath(dataDirPath);
    if(dataDir.exists("Data"))
    {
        QMessageBox::warning(this,"Warning!","Can not creat the Data Dir for this LINE!");
        return;
    }

    if(!dataDir.mkdir("Data"))
        qDebug()<<"Creat Line Data Dir failed";

    QFile dataDesc;
    dataDesc.setFileName(dataDir.path()+"/Data/DescName");
    if(!dataDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat data dir desc open failed";
    }
    QTextStream datats(&dataDesc);
    datats<<"Data\n";
    dataDesc.close();

    // 4. 创建各个类型文件的文件夹
    QStringList fileTypeList=settings.value("FileType").toStringList();
    for(int i=0;i<fileTypeList.size();i++)
    {
        QDir typeDir;
        QString typeDirPath=dataDirPath+"/Data";
        typeDir.setPath(typeDirPath);
        if(typeDir.exists(fileTypeList.at(i)))
        {
            QMessageBox::warning(this,"Warning!","Can not creat the Same Type Dir for this LINE Data!");
            return;
        }

        if(!typeDir.mkdir(fileTypeList.at(i)))
            qDebug()<<"Creat Line Data Dir failed";
    }


    setHomeDir(getHomeDir());
    areaWidget->expandToDepth(1);
}

void QJDMainWindow::creatNewFlow(QString flowName)
{
    QString lowerCaseFlowName;
    lowerCaseFlowName=flowName.toLower();
    lowerCaseFlowName.simplified();
    lowerCaseFlowName.remove(" ");
    qDebug()<<"creatNewFlow::"<<flowName<<lowerCaseFlowName;  // 所有大写阿,带空格的之类的统一小写,并且去空格

    QString absolutePath;
    if(areaWidget->level()==2)
    {
        absolutePath=areaWidget->getAbsolutePath();
    }
    if(areaWidget->level()==3)
    {
        // flowpath,需要去掉现有的flow
        absolutePath=areaWidget->getAbsolutePath();
        absolutePath.chop(areaWidget->getAbsolutePath().size()-areaWidget->getAbsolutePath().lastIndexOf("/"));
    }

    //1. 创建文件夹
    QDir newDir;
    newDir.setPath(absolutePath);
    if(newDir.exists(lowerCaseFlowName))
    {
        QMessageBox::warning(this,"Warning!","Do not creat the same FLOW again!");
        canCreatNewFlow=false;
        return;
    }
    if(!newDir.mkdir(lowerCaseFlowName))
    {
        qDebug()<<"Creat New Flow Dir failed";
    }
    else
    {
        QDir logDir;
        QString logDirPath;
        logDirPath=absolutePath+"/"+lowerCaseFlowName;
        qDebug()<<"logDirPath::"<<logDirPath;
        logDir.setPath(logDirPath);

        if(!logDir.mkdir("log"))
        {
            qDebug()<<"Creat New Flow Log Dir failed";
        }
    }

    canCreatNewFlow=true;
    //2. 创建.Desc
    QFile newDesc;
    newDesc.setFileName(newDir.path()+"/"+lowerCaseFlowName+"/DescName");
    if(!newDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat new desc open failed";
    }
    QTextStream ts(&newDesc);
    ts<<flowName<<"\n";
    newDesc.close();
}

void QJDMainWindow::on_actionExcuteFlow_triggered()
{
    qDebug()<<".::::::::::::excute this flow:::::::::::.";

    //    1. 读取listwidget顺序,并且保存到pos;
    //    2. 按照顺序提取各个xml内容并输出到输出文件;
    //    3. 调用程序
    /// 应当传输信号进mdi,或者mdi传出??
    mdiWidget->excuteFlow(mdiWidget->currentSubWindow()->windowTitle());

}

/// 删除非空目录
void QJDMainWindow::deleteDir(QFileInfo fileList)
{
    if(fileList.isDir())
    {
        int childCount =0;
        QString dir = fileList.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount>2)
        {
            for(int i=0;i<childCount;i++)
            {
                if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==(".."))
                {
                    continue;
                }
                deleteDir(newFileList.at(i));
            }
        }
        fileList.absoluteDir().rmpath(fileList.fileName());
    }
    else if(fileList.isFile())
    {
        fileList.absoluteDir().remove(fileList.fileName());
    }
}

void QJDMainWindow::openFlowSlot()
{
    showExistFlow();
}

void QJDMainWindow::excuteFlowSlot()
{
    /// 先显示,再执行,有问题
    showExistFlow();
    mdiWidget->excuteFlow(mdiWidget->currentSubWindow()->windowTitle());
}

void QJDMainWindow::on_actionJob_Viewer_triggered()
{
    mdiWidget->showProcessWidget();
}

void QJDMainWindow::excuteDecide(QMdiSubWindow *w)
{
//    qDebug()<<"excuteDecide!!!!!"<<w;
    if(w==0)
    {
        ui->actionExcuteFlow->setEnabled(false);
//        tabWidget->setCurrentIndex(0);
    }
    else
    {
        ui->actionExcuteFlow->setEnabled(true);
    }
}

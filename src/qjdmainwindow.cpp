#include "qjdmainwindow.h"
#include "ui_qjdmainwindow.h"
#include <QDebug>
#include <QLayout>
#include <QFileSystemModel>
#include <QSettings>
#include <QFileDialog>

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

    QCoreApplication::setOrganizationName("JDSoft");
    QCoreApplication::setOrganizationDomain("jindisoft.com");
    QCoreApplication::setApplicationName("Flow2");
    QSettings settings;
    settings.setValue("test1",12);
    int value=settings.value("test1").toInt();
    qDebug()<<"value"<<value;

    pathLabel1=new QJDLabel;
    pathLabel1->setText("Address::");

    pathLabel2=new QJDLabel;
    pathLabel2->setFrameShape(QFrame::Box);
    pathLabel2->setFrameShadow(QFrame::Raised);
    pathLabel2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    areaHeadWidget=new QJDAreaHeadWidget;
    areaWidget=new QJDAreaWidget();
    areaWidget->setColumnCount(1);
    connect(areaWidget,SIGNAL(sigLevel(int)),this,SLOT(enableNew(int)));

    propertyWidget=new QJDPropertyWidget;
    mdiWidget=new QJDMdi; // connect with areawidget & funcationwidget

    tabWidget=new QJDTabWidget();
//    tabWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    QWidget *tab1=new QWidget;
    QGridLayout *gLayout1=new QGridLayout(tab1);
    gLayout1->addWidget(propertyWidget);
    tabWidget->addTab(tab1, "Property Viewer");
    QWidget *tab2=new QWidget;
    QGridLayout *gLayout2=new QGridLayout(tab2);
    gLayout2->addWidget(mdiWidget);
    tabWidget->addTab(tab2, "Flow Editor");
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

    setHomeDir("/home/xtf/Data/");
    _HOME_DIR="/home/xtf/Data/";
    areaWidget->setHome(_HOME_DIR);

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
    connect(areaWidget,SIGNAL(sigItemPath(QString)),pathLabel2,SLOT(setText(QString)));
    connect(areaWidget,SIGNAL(sigItemPath(QString)),propertyWidget,SLOT(setPropertyData(QString)));
    connect(areaWidget,SIGNAL(sigActNewAreaClicked()),this,SLOT(on_actionNewArea_triggered()));
    connect(areaWidget,SIGNAL(sigActNewLineClicked()),this,SLOT(on_actionNewLine_triggered()));
    connect(areaWidget,SIGNAL(sigActNewFlowClicked()),this,SLOT(on_actionNewFlow_triggered()));
    connect(areaWidget,SIGNAL(sigExistFlow()),this,SLOT(showExistFlow()));

    // funcation
    connect(funcationWidget,SIGNAL(sigFunDoubleClicked(QString,QString)),mdiWidget,SLOT(addFlow(QString,QString)));
//    connect(funcationWidget,SIGNAL(sigFunDoubleClicked(QString,QString)),this,SLOT(copyFlowXML()));

    // head
    connect(areaHeadWidget,SIGNAL(sigExpandClicked()),areaWidget,SLOT(expandAll()));
    connect(areaHeadWidget,SIGNAL(sigCollapseClicked()),areaWidget,SLOT(collapseAll()));
    connect(funcationHeadWidget,SIGNAL(sigExpandClicked()),funcationWidget,SLOT(expandAll()));
    connect(funcationHeadWidget,SIGNAL(sigCollapseClicked()),funcationWidget,SLOT(collapseAll()));

    /// mdi
//    connect(areaWidget,SIGNAL(),mdiWidget,SLOT(newSubWindow()));
//    connect(funcationWidget,SIGNAL(),mdiWidget,SLOT(addFlow()));

    showMaximized();
}

QJDMainWindow::~QJDMainWindow()
{
    delete ui;
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

    /// 第二层 -- 线
    QStringList areaStringList;
    QStringList areaPathList;
    QStringList lineStringList;
    QStringList linePathList;
//    QStringList flowStringList;
//    QStringList flowPathList;
    QVector<QStringList> flowStringList;
    QVector<QStringList> flowPathList;
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

        /// 第三层 -- 流程
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
            }
        }
//        qDebug()<<"\n**begin**"<<i<<"area string::"<<areaStringList.at(i)<<"area path::"<<areaPathList.at(i);
//        qDebug()<<"line string::"<<lineStringList.at(j)<<"line path::"<<linePathList.at(j);
//        qDebug()<<"flow string::"<<flowStringList<<"flow path::"<<flowPathList<<"**end**";

        setAreaWidget(areaStringList.at(i),areaPathList.at(i),lineStringList,
                      linePathList,flowStringList,flowPathList);

        flowStringList.clear();
        flowPathList.clear();
        lineStringList.clear();
        linePathList.clear();
    }
}

void QJDMainWindow::setAreaWidget(QString areaString, QString areaPath, QStringList lineStringList,
                                  QStringList linePathList, QVector<QStringList> flowStringList, QVector<QStringList> flowPathList)
{
    qDebug()<<"setAreaWidget::"<<flowStringList<<flowPathList;
    QTreeWidgetItem *areaItem = new QTreeWidgetItem;
    areaItem->setText(0,areaString);
    areaItem->setToolTip(0,areaPath);

    for(int j=0;j<lineStringList.count();j++)
    {
        QTreeWidgetItem *lineItem = new QTreeWidgetItem;
        lineItem->setText(0,lineStringList.at(j));
        lineItem->setToolTip(0,linePathList.at(j));

        for(int k=0;k<flowStringList[j].size();k++)
        {
            QTreeWidgetItem *flowItem = new QTreeWidgetItem;
            flowItem->setText(0,flowStringList[j].at(k));
            flowItem->setToolTip(0,flowPathList[j].at(k));
            lineItem->addChild(flowItem);
        }

        areaItem->addChild(lineItem);
    }
    areaWidget->addTopLevelItem(areaItem);
}

void QJDMainWindow::on_actionExit_triggered()
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
        qDebug()<<"area enable | line enable??";
        ui->actionNewArea->setEnabled(true);
        ui->actionNewLine->setEnabled(true);
        ui->actionNewFlow->setEnabled(false);
    }
    if(level==2)
    {
        qDebug()<<"line & flow enable";
        ui->actionNewArea->setEnabled(false);
        ui->actionNewLine->setEnabled(true);
        ui->actionNewFlow->setEnabled(true);
    }
    if(level==3)
    {
        qDebug()<<"flow enable";
        ui->actionNewArea->setEnabled(false);
        ui->actionNewLine->setEnabled(false);
        ui->actionNewFlow->setEnabled(true);
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
    connect(newFlow,SIGNAL(sigFlowName(QString)),this,SLOT(creatNewFlow(QString)));
    newFlow->exec();
    // ---------------------------------------------------------------------- //
    // 模态,以下语句将在已返回的情况下执行,除了需要名称之外,路径也需要
    qDebug()<<"after new flow->exec!"<<newFlow->getFlowName()<<areaWidget->getAbsolutePath();
    // 需要先确定absloutePath是flowpath还是linepath
    if(areaWidget->level()==2)
    {
        // linepath
        qDebug()<<"IN level 2::"<<areaWidget->getAbsolutePath();
        mdiWidget->newSubWindow(newFlow->getFlowName(),areaWidget->getAbsolutePath());
    }
    if(areaWidget->level()==3)
    {
        // flowpath,需要去掉现有的flow
        QString linePath=areaWidget->getAbsolutePath();
        linePath.chop(areaWidget->getAbsolutePath().size()-areaWidget->getAbsolutePath().lastIndexOf("/"));
        qDebug()<<"IN level 3"<<linePath;
        mdiWidget->newSubWindow(newFlow->getFlowName(),linePath);
    }
}

void QJDMainWindow::showExistFlow()
{
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
        _HOME_DIR=homePath;
        areaWidget->setHome(_HOME_DIR);
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
    ts<<areaName;
    newDesc.close();

    //3. 全局刷一下列表呢?还是添加到列表???
    setHomeDir(getHomeDir());  // ??这样来不负责任通刷??
}

void QJDMainWindow::creatNewLine(QString lineName)
{
    QString lowerCaseLineName;
    lowerCaseLineName=lineName.toLower();
    lowerCaseLineName.simplified();
    lowerCaseLineName.remove(" ");
    qDebug()<<"creatNewArea::"<<lineName<<lowerCaseLineName;  // 所有大写阿,带空格的之类的统一小写,并且去空格

    //1. 创建文件夹,需要获取当前area文件夹名称,通过当前的名称获取
    QDir newDir;
    newDir.setPath(areaWidget->getAbsolutePath());
    if(!newDir.mkdir(lowerCaseLineName))
        qDebug()<<"Creat New Line Dir failed";

    //2. 创建.Desc
    QFile newDesc;
    newDesc.setFileName(newDir.path()+"/"+lowerCaseLineName+"/DescName");
    if(!newDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat new desc open failed";
    }
    QTextStream ts(&newDesc);
    ts<<lineName;
    newDesc.close();

    //3. 全局刷一下列表呢?还是添加到列表???
    setHomeDir(getHomeDir());
}

void QJDMainWindow::creatNewFlow(QString flowName)
{
    QString lowerCaseFlowName;
    lowerCaseFlowName=flowName.toLower();
    lowerCaseFlowName.simplified();
    lowerCaseFlowName.remove(" ");
    qDebug()<<"creatNewArea::"<<flowName<<lowerCaseFlowName;  // 所有大写阿,带空格的之类的统一小写,并且去空格

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
    if(!newDir.mkdir(lowerCaseFlowName))   // 工作正常??
        qDebug()<<"Creat New Flow Dir failed";

    //2. 创建.Desc
    QFile newDesc;
    newDesc.setFileName(newDir.path()+"/"+lowerCaseFlowName+"/DescName");
    if(!newDesc.open(QFile::WriteOnly))
    {
        qDebug()<<"creat new desc open failed";
    }
    QTextStream ts(&newDesc);
    ts<<flowName;
    ts<<"\n";
    newDesc.close();

    //3. 全局刷一下列表呢?还是添加到列表???
    setHomeDir(getHomeDir());
}

void QJDMainWindow::on_actionExcuteFlow_triggered()
{
    qDebug()<<":::::::::::::excute this flow::::::::::::";

    //    1. 读取listwidget顺序,并且保存到pos;
    //    2. 按照顺序提取各个xml内容并输出到输出文件;
    //    3. 调用程序
    /// 应当传输信号进mdi,或者mdi传出??
    mdiWidget->excuteFlow();  //

}

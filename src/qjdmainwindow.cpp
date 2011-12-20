#include "qjdmainwindow.h"
#include "ui_qjdmainwindow.h"
#include <QDebug>
#include <QLayout>
#include <QFileSystemModel>
#include <QSettings>

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

    areaWidget=new QJDTreeWidget;
    areaWidget->setColumnCount(1);

    propertyWidget=new QJDPropertyWidget;
    mdiWidget=new QJDMdi;

    tabWidget=new QJDTabWidget;
    QWidget *tab1=new QWidget;
    QGridLayout *gLayout1=new QGridLayout(tab1);
    gLayout1->addWidget(propertyWidget);
    tabWidget->addTab(tab1, "Property Viewer");

    QWidget *tab2=new QWidget;
    QGridLayout *gLayout2=new QGridLayout(tab2);
    gLayout2->addWidget(mdiWidget);
    tabWidget->addTab(tab2, "Flow Editor");

    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(setWidgetVisible(int)));

    functionWidget=new QJDFuncationWidget;
    functionWidget->setVisible(false);

    setDir();
    QHBoxLayout *pathLayout=new QHBoxLayout;
    pathLayout->addWidget(pathLabel1);
    pathLayout->addWidget(pathLabel2);

    QHBoxLayout *hLayout=new QHBoxLayout;
    hLayout->addWidget(areaWidget);
    hLayout->addWidget(tabWidget);
//    hLayout->addWidget(propertyWidget);  // 不是应该在tabwidget里面么
    hLayout->addWidget(functionWidget);

    QVBoxLayout *vLayout=new QVBoxLayout;
    vLayout->addLayout(pathLayout);
    vLayout->addLayout(hLayout);

    ui->centralWidget->setLayout(vLayout);

    connect(areaWidget,SIGNAL(sigItemPath(QString)),pathLabel2,SLOT(setText(QString)));
    connect(areaWidget,SIGNAL(sigItemPath(QString)),propertyWidget,SLOT(setPropertyData(QString)));

    showMaximized();
}

QJDMainWindow::~QJDMainWindow()
{
    delete ui;
}

/// TODO:名称和路径需要联系起来
// 不能使用QHash ,会出现string相同的情况,那用什么方法呢
// QMultiMap??
void QJDMainWindow::setDir()
{
    /// 第一层 -- 工区
    QDir dir1;
    dir1.setPath("/home/xtf/PromaxData/");  // 这个需要能设置,程序需要有settings.ini
    QStringList dirLev1;
    dirLev1<<dir1.entryList(QDir::NoDotAndDotDot|QDir::Dirs);
//    qDebug()<<dir1.count(); // 包含./..

    /// 第二层 -- 线
    QStringList areaStringList;
    QStringList areaPathList;
    QStringList lineStringList;
    QStringList linePathList;
    QStringList flowStringList;
    QStringList flowPathList;
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
            for(int k=0;k<dirLev3.count();k++)
            {
                // 应当没有文件夹了,只剩下文件了
                QDir dir4;
                QString dir4path=dir3.path()+"/"+dirLev3.at(k);
                dir4.setPath(dir4path);
                QStringList dirLev4;
                dirLev4=dir4.entryList(QDir::NoDotAndDotDot|QDir::Files);  // 文件名列表了
                flowPathList<<dir4path;
                /// 底下应当有个记录流程xml文件

                // 解析 DescName -- 线名称
                QFile file4;
                file4.setFileName(dir4path+"/DescName");
                file4.open(QFile::ReadOnly);
                QTextStream stream4(&file4);
                QString flowtmp=stream4.readAll();
                flowtmp.chop(1);
                flowStringList<<flowtmp;  // 只有在流程里才会有,其他的文件夹内不会有这个
                file4.close();
//                qDebug()<<"flow::"<<flowStringList;
            }
        }
        setAreaWidget(areaStringList.at(i),areaPathList.at(i),lineStringList,linePathList,flowStringList,flowPathList);
        lineStringList.clear();
        flowStringList.clear();
    }
}

void QJDMainWindow::setAreaWidget(QString areaString, QString areaPath, QStringList lineStringList,
                                  QStringList linePathList, QStringList flowStringList, QStringList flowPathList)
{
    QTreeWidgetItem *areaItem = new QTreeWidgetItem;
    areaItem->setText(0,areaString);
    areaItem->setToolTip(0,areaPath);

    for(int j=0;j<lineStringList.count();j++)
    {
        QTreeWidgetItem *lineItem = new QTreeWidgetItem;
        lineItem->setText(0,lineStringList.at(j));
        lineItem->setToolTip(0,linePathList.at(j));

        for(int k=0;k<flowStringList.count();k++)
        {
            QTreeWidgetItem *flowItem = new QTreeWidgetItem;
            flowItem->setText(0,flowStringList.at(k));
            flowItem->setToolTip(0,flowPathList.at(k));

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
//    qDebug()<<tabIndex;
    if(tabIndex==0)  // Property
    {
        functionWidget->setVisible(false);
    }
    if(tabIndex==1)  // Flow
    {
        functionWidget->setVisible(true);
    }
}

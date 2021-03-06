#include "qjdfilechooseread.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>
#include <QDebug>

QJDFileChooseRead::QJDFileChooseRead(QWidget *parent) :
    QWidget(parent)
{    
    propertyInt=-1;

    typeLabel=new QLabel;
    listLabel=new QLabel;
    pathLabel=new QLabel;
    typeLabel->setText("Choose Data Type::");
    listLabel->setText("Choose File List::");
    leftListWidget=new QListWidget;
    rightListWidget=new QListWidget;

    QVBoxLayout *vLayout=new QVBoxLayout;
    QGridLayout *gLayout=new QGridLayout;
    gLayout->addWidget(typeLabel,0,0);
    gLayout->addWidget(listLabel,0,1);
    gLayout->addWidget(leftListWidget,1,0);
    gLayout->addWidget(rightListWidget,1,1);

    vLayout->addLayout(gLayout);
    vLayout->addWidget(pathLabel);

    this->setLayout(vLayout);

    connect(leftListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(setFileList(QListWidgetItem*)));
    connect(rightListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(emitEditFinished(QListWidgetItem*)));
}

void QJDFileChooseRead::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

// 读取data目录下的文件夹
void QJDFileChooseRead::setTypeData(QString dataDirPath)
{
    DATA_PATH=dataDirPath;
    qDebug()<<"QJDFileChooseRead::setTypeData"<<dataDirPath;
    QDir dataDir(dataDirPath);
    QStringList dataDirTypeList;
    dataDirTypeList<<dataDir.entryList(QDir::NoDotAndDotDot|QDir::Dirs);

    QFileInfo f(VALUE);
    for(int i=0;i<dataDirTypeList.size();i++)
    {
        QListWidgetItem *itemType=new QListWidgetItem;
        itemType->setText(dataDirTypeList.at(i));
        leftListWidget->addItem(itemType);
        if(DATATYPE==dataDirTypeList.at(i))
        {
            leftListWidget->setCurrentItem(itemType);
            setFileList(itemType);
            for(int j=0;j<rightListWidget->count();j++)
            {
//                qDebug()<<"!!!!!!!!!!"<<f.fileName()<<rightListWidget->item(j)->text();
                if(f.fileName()==rightListWidget->item(j)->text())
                {
                    rightListWidget->setCurrentRow(j);
                }
            }
        }
    }
}

void QJDFileChooseRead::setFileList(QListWidgetItem *item)
{
    rightListWidget->clear();
    qDebug()<<"QJDFileChooseRead::setFileList"<<item->text();
    QDir typeDir;
    QString dirPath=DATA_PATH+"/"+item->text();
    DIR_PATH=dirPath;
    typeDir.setPath(dirPath);
    QStringList typeDirList;
    typeDirList<<typeDir.entryList(QDir::NoDotAndDotDot|QDir::Files);
    for(int i=0;i<typeDirList.size();i++)
    {
        QListWidgetItem *itemType=new QListWidgetItem;
        itemType->setText(typeDirList.at(i));
        rightListWidget->addItem(itemType);
    }
}

void QJDFileChooseRead::emitEditFinished(QListWidgetItem *item)
{
    QString type=DIR_PATH.right(DIR_PATH.count()-DIR_PATH.lastIndexOf("/")-1);
    QString value=DIR_PATH+"/"+item->text();
    pathLabel->setText(value);
    QStringList list;
    list<<PROPERTY<<DESC<<type<<DISPLAYTYPE<<value<<OPTION;
    emit sigFileChooseReadEditChanged(propertyInt,list);
}

void QJDFileChooseRead::setCurrentValue(QString value)
{
    VALUE=value;
    pathLabel->setText(value);
}


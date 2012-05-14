#include "qjdpropertywidget.h"
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

QJDPropertyWidget::QJDPropertyWidget()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/// 列出给定路径下一层的文件或文件夹相关信息
// 缺陷:1.name 只有descname才能显示,否则没有,size目前只有目录有size,文件没有
void QJDPropertyWidget::setPropertyData(QString path)
{
    QStandardItemModel *model=new QStandardItemModel();
    //设置大小
    model->setColumnCount(5);    //列
    model->setRowCount(2);    //行

    //设置标题
    QStringList a;
//    a<<"DescName"<<"Owner"<<"Modified"<<"Size"<<"File";
    a<<"File"<<"DescName"<<"Owner"<<"Modified"<<"Size";
    model->setHorizontalHeaderLabels(a);

    QDir dir1;
    dir1.setPath(path);  // 这个需要能设置,程序需要有settings.ini
    QStringList dirLev1;
    dirLev1<<dir1.entryList(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files);

    qDebug()<<"dirLev1::"<<dirLev1;
    for(int i=0;i<dirLev1.count();i++)
    {
        QFileInfo *fileInfo=new QFileInfo;
        QString dir2path=dir1.path()+"/"+dirLev1.at(i);
        fileInfo->setFile(dir2path);

        // 解析 DescName -- 工区名称
        QFile file2;
        file2.setFileName(dir2path+"/DescName");
        file2.open(QFile::ReadOnly);
        QTextStream stream2(&file2);
        QString areatmp=stream2.readAll();
        areatmp.chop(1);
//        qDebug()<<areatmp;
        file2.close();

        QStandardItem *itemDesc = new QStandardItem(areatmp);
        QStandardItem *itemOwner = new QStandardItem(fileInfo->owner());
        QStandardItem *itemModify = new QStandardItem(fileInfo->lastModified().toString("yyyy-MM-dd h:mm:ss"));
        QFileInfo info(dir1.path()+"/"+dirLev1.at(i));
        QStandardItem *itemSize;
        if(info.isDir()) // 是目录
        {
            itemSize = new QStandardItem(dirSize(dir1.path()+"/"+dirLev1.at(i)));
        }
        if(info.isFile()) // 是文件
        {

            itemSize = new QStandardItem(fileSize(info.size()));
        }
        QStandardItem *itemFileName = new QStandardItem(fileInfo->fileName());
        model->setItem(i,0,itemFileName);
        model->setItem(i,1,itemDesc);
        model->setItem(i,2,itemOwner);
        model->setItem(i,3,itemModify);
        model->setItem(i,4,itemSize);

    }


    //选择这个model
    this->setModel(model);

    //隐藏左边那列
    this->verticalHeader()->hide();

    this->resizeColumnsToContents();
    this->horizontalHeader()->setStretchLastSection(true);

    this->setSortingEnabled(true);       //自动排序关闭
    this->horizontalHeader()->setSortIndicatorShown(true);
    this->horizontalHeader()->setClickable(true);


    //列宽
//    this->setColumnWidth(0,30);
    //整行选择
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

QString QJDPropertyWidget::dirSize(const QString &sDirPath)
{
    qint64 iSize = 0;
    QDir o_dir(sDirPath);
    if (!o_dir.exists())
    {
        return 0;
    }

    QStringList slst_dir_path;
    slst_dir_path << sDirPath;
    while (slst_dir_path.isEmpty() == false)
    {
        o_dir.setPath(slst_dir_path[0]);
        o_dir.setFilter(QDir::Dirs | QDir::Files);
        o_dir.setSorting(QDir::DirsFirst);
        slst_dir_path.removeFirst();

        foreach (QFileInfo o_file_info, o_dir.entryInfoList())
        {
            if (o_file_info.fileName() == "." || o_file_info.fileName() == "..")
            {
                continue;
            }

            iSize += o_file_info.size();
            if (o_file_info.isDir())
            {
                slst_dir_path << o_file_info.filePath();
            }
        }
    }

    QString dirWholeSize;

    int dirsize=iSize;
    if(dirsize<1024)
    {
        int tmp;
        tmp=dirsize;
        dirWholeSize=QString::number(tmp);
        dirWholeSize.append("B");
    }
    if(dirsize>=1024 && dirsize<1048576)
    {
        int tmp;
        tmp=dirsize/1024;
        dirWholeSize=QString::number(tmp);
        dirWholeSize.append("KB");
    }
    if(dirsize>=1048576 && dirsize<1073741824)
    {
        int tmp;
        tmp=dirsize/1024/1024;
        dirWholeSize=QString::number(tmp);
        dirWholeSize.append("MB");
    }
    if(dirsize>=1073741824)
    {
        int tmp;
        tmp=dirsize/1024/1024/1024;
        dirWholeSize=QString::number(tmp);
        dirWholeSize.append("GB");
    }
    return dirWholeSize;
}

QString QJDPropertyWidget::fileSize(qint64 size)
{
    QString fileSize;

    if(size<1024)
    {
        int tmp;
        tmp=size;
        fileSize=QString::number(tmp);
        fileSize.append("B");
    }
    if(size>=1024 && size<1048576)
    {
        int tmp;
        tmp=size/1024;
        fileSize=QString::number(tmp);
        fileSize.append("KB");
    }
    if(size>=1048576 && size<1073741824)
    {
        int tmp;
        tmp=size/1024/1024;
        fileSize=QString::number(tmp);
        fileSize.append("MB");
    }
    if(size>=1073741824)
    {
        int tmp;
        tmp=size/1024/1024/1024;
        fileSize=QString::number(tmp);
        fileSize.append("GB");
    }
    return fileSize;

}


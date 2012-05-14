#include "qjdmdi.h"
#include <QMdiSubWindow>
#include "qjdmdisublistwidget.h"
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QMouseEvent>
#include "qjdfunarguwidget.h"
#include "qjdprocesswidget.h"

/// 这是个很麻烦的部分
// 1.通关新建或者打开流程在里面建立窗口
/// TODO: 添加函数 1.new flow 外部传参,新建窗口 2.添加流程 funcation widget 传进来 3.flow 的 xml 读取
QJDMdi::QJDMdi()
{
    moduleName.clear();
    propertyNameAndValue.clear();
    propertyNameAndValueList.clear();

    setAttribute(Qt::WA_DeleteOnClose);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // 没有用?
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    /// 添加sub要信号传过来再添加,sub中的内容则靠funcation list
    // 从area widget当中 新建 flow 传过来
//    QJDMdiSubListWidget *tab1=new QJDMdiSubListWidget;
//    tab1->setMinimumSize(200,200);
//    tab1->setWindowTitle("test1");

//    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
//    subWindow1->setWidget(tab1);
//    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
//    this->addSubWindow(subWindow1);

//    connect(this,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(windowChoosed(QMdiSubWindow*)));
}

void QJDMdi::windowChoosed(QMdiSubWindow *window)
{
    qDebug()<<"QJDMdi::windowChoosed::"<<window;
}

/// Q:在切换到mdi显示的时候,无法正常添加,但切换出去添加,切换回来正常,WHY????
/// A: subwindow->show()
void QJDMdi::newSubWindow(QString subName,QString linePath)
{
    // area widget 新建flow,这里就需要新建一个subwindow
    // 在mainwindow中联系,subwindow最好自带路径,否则不便
    qDebug()<<"QJDMdi::newSubWindow"<<subName;
    QJDMdiSubListWidget *listWidget=new QJDMdiSubListWidget;
    listWidget->setMinimumSize(200,200);

    // Area::Line::Flow
    QStringList areaLineFlow=linePath.split("/",QString::SkipEmptyParts);
    QString areaString=areaLineFlow.at(areaLineFlow.size()-2);
    QString lineString=areaLineFlow.at(areaLineFlow.size()-1);
    listWidget->setWindowTitle(areaString+"::"+lineString+"::"+subName);

    QString flowPath=linePath+"/"+subName;
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWindowIcon(QIcon(":/src/images/flowviewer"));
    subWindow->setToolTip(flowPath);
    subWindow->setWidget(listWidget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->setWindowFlags(Qt::Window
                              | Qt::WindowMinimizeButtonHint
                              | Qt::WindowCloseButtonHint);
    hashWindow_List.insert(subWindow,listWidget);
    this->addSubWindow(subWindow);
    this->setActiveSubWindow(subWindow);

    /// 中键显示参数窗口
    connect(listWidget,SIGNAL(sigMidButtonClicked(QListWidgetItem*)),
            this,SLOT(listWidgetItemMidClicked(QListWidgetItem*)));
    /// 右键显示功能菜单
    connect(listWidget,SIGNAL(sigDelFlowClicked(QList<QListWidgetItem *>)),
             this,SLOT(delFlow(QList<QListWidgetItem *>)));
    connect(listWidget,SIGNAL(sigOpenCloseFlowClicked(QList<QListWidgetItem *>)),
             this,SLOT(openCloseFlow(QList<QListWidgetItem *>)));

    subWindow->show();  //正常显示,new的时候不需要在listwidget中显示内容,但是从area双击过来的需要解析并且显示
}

// 仅做显示用,无需处理添加删除,均交由最后提交处理
void QJDMdi::showExistSubWindow(QString flowName, QString flowPath)
{
    qDebug()<<flowName<<flowPath;  //那可是要从flowPath下面的pos文件里面读取顺序
    /// 0. 先要进行重复处理,对flowPath进行审查
    QJDMdiSubListWidget *listWidget=new QJDMdiSubListWidget;
    listWidget->setMinimumSize(200,200);

    // Area::Line::Flow
    QStringList areaLineFlow=flowPath.split("/",QString::SkipEmptyParts);
    QString areaString=areaLineFlow.at(areaLineFlow.size()-3);
    QString lineString=areaLineFlow.at(areaLineFlow.size()-2);
    listWidget->setWindowTitle(areaString+"::"+lineString+"::"+flowName);

    /// 1. 读取顺序文件,写入list
    QFile posFile;
    QString posFileName=flowPath+"/pos";
    posFile.setFileName(posFileName);
    if(!posFile.open(QFile::ReadOnly))
    {
        qDebug()<<"pos file open failed or doesn`t exist.";
    }
    QTextStream ts(&posFile);
    QStringList funList;
    QStringList funPathList;
    QStringList funStatusList;

    QString funLine=ts.readLine();
    QString funPathLine=ts.readLine();
    QString funStatusLine=ts.readLine();

    funList=funLine.split(";",QString::SkipEmptyParts);
    funPathList=funPathLine.split(";",QString::SkipEmptyParts);
    funStatusList=funStatusLine.split(";",QString::SkipEmptyParts);

//    qDebug()<<"funlist"<<funList;
//    qDebug()<<"funPathList"<<funPathList;
    if(funList.count()==funPathList.count())
    {
        for(int i=0;i<funList.count();i++)
        {
            QListWidgetItem *listItem=new QListWidgetItem;
            listItem->setText(funList.at(i));
            listItem->setToolTip(funPathList.at(i));
            listItem->setStatusTip(funStatusList.at(i));
            listItem->setIcon(QIcon(":/src/images/run.png"));
            if(funStatusList.at(i)=="0")
            {
                listItem->setBackgroundColor(QColor(180,180,180));
                listItem->setIcon(QIcon(":/src/images/norun.png"));
            }
            listWidget->addItem(listItem);
        }
        if(funList.count()!=0)
        {
            listWidget->setCurrentRow(0);
        }
    }
    /// 中键显示参数窗口
    connect(listWidget,SIGNAL(sigMidButtonClicked(QListWidgetItem*)),
            this,SLOT(listWidgetItemMidClicked(QListWidgetItem*)));
    /// 右键显示功能菜单
    connect(listWidget,SIGNAL(sigDelFlowClicked(QList<QListWidgetItem *>)),
             this,SLOT(delFlow(QList<QListWidgetItem *>)));
    connect(listWidget,SIGNAL(sigOpenCloseFlowClicked(QList<QListWidgetItem *>)),
             this,SLOT(openCloseFlow(QList<QListWidgetItem *>)));

    /// 2.  创建subWindow,如果重复了,要有重复处理
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWindowIcon(QIcon(":/src/images/flowviewer"));
    subWindow->setToolTip(flowPath);  // 依据tooltip来判断是否重复,不能依据名字来判断重复
    subWindow->setWidget(listWidget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->setWindowFlags(Qt::Window
                              | Qt::WindowMinimizeButtonHint
                              | Qt::WindowCloseButtonHint);
    hashWindow_List.insert(subWindow,listWidget);
    this->addSubWindow(subWindow);
    this->setActiveSubWindow(subWindow);
    subWindow->show();  //正常显示,new的时候不需要在listwidget中显示内容,但是从area双击过来的需要解析并且显示

    refreshPosFile();  // 往内存中加点料
}


void QJDMdi::addFlow(QString name, QString xmlPath)
{
    //    1. 传输名字
    //    2. 复制xml 文件到本flow文件中去!单独文件,无需在意位置,原始文件直接复制
    //    3. 位置的串联由顺序决定
    //    4. 已有的flow则是扫描顺序文件?顺序文件采取何种方式
    // -----------------------------------------------------------------------------------------------------//
    qDebug()<<"QJDMdi::addFlow"<<name<<xmlPath;
    /// 在当前的mdi窗口中,添加一行,需要找到那个listwidget,QHASH(sub,list),great idea
    QMdiSubWindow *subWindow=currentSubWindow();


    /// 复制文件到当前flow
    // from listwidgetitem tootip To subwindow tooltip
    QString copyFromFile="fun/"+xmlPath;
    QString copyToFile=subWindow->toolTip(); // +文件名
    QString xmlFileName=xmlPath.right( xmlPath.size()-xmlPath.lastIndexOf("/") -1);
    copyToFile=copyToFile+"/"+xmlFileName;  /// !!!!如何保证不重???
    qDebug()<<"copy xml file"<<copyFromFile<<copyToFile;

    /// 检查重复
    QString  copyToFileName;
    copyToFileName=checkFileIfExist(copyToFile);  // 已经做了连续检查

    /// 复制文件
    QFile xmlFile(copyFromFile);
    if(xmlFile.exists())
    {
        if(xmlFile.copy(copyToFileName)==false)
        {
            qDebug()<<"copy xml file failed";
        }
    }
    else
    {
        qDebug()<<"copyFromFile doesn`t exist";
    }

    QJDMdiSubListWidget *listWidget=hashWindow_List.value(subWindow);
    QListWidgetItem *listWidgetItem=new QListWidgetItem;
    listWidgetItem->setText(name);
    QString listToolTip=copyToFileName.right( copyToFileName.size()-copyToFileName.lastIndexOf("/") -1);
    listWidgetItem->setToolTip(listToolTip);  // 应当是fileToName的cut版
    listWidgetItem->setStatusTip("1"); // 默认打开
    listWidgetItem->setIcon(QIcon(":/src/images/run.png"));
    /// 插入到当前的后者会比较好
//    listWidget->addItem(listWidgetItem);
    listWidget->insertItem(listWidget->currentRow()+1,listWidgetItem);

//    /// 中键显示参数窗口
//    connect(listWidget,SIGNAL(sigMidButtonClicked(QListWidgetItem*)),
//            this,SLOT(listWidgetItemMidClicked(QListWidgetItem*)));
//    /// 右键显示功能菜单
//    connect(listWidget,SIGNAL(sigDelFlowClicked(QList<QListWidgetItem *>)),
//             this,SLOT(delFlow(QList<QListWidgetItem *>)));
//    connect(listWidget,SIGNAL(sigOpenCloseFlowClicked(QList<QListWidgetItem *>)),
//             this,SLOT(openCloseFlow(QList<QListWidgetItem *>)));

    refreshPosFile();
}

/// 需要链接到右键
bool QJDMdi::delFlow(QList<QListWidgetItem *> itemList)
{
    // 1. 找到当前flow
    // 2. 删除list
    // 3. 删除文件
    qDebug()<<"\nQJDMdi::delFlow::"<<itemList;  // 怎么会删除两次
    QMdiSubWindow *subWindow=currentSubWindow();
    QJDMdiSubListWidget *listWidget=hashWindow_List.value(subWindow);
//    listWidget->setCurrentItem(item);

    for(int i=0;i<itemList.size();i++)
    {
        QListWidgetItem *listWidgetItem=itemList.at(i);
        listWidget->setCurrentItem(listWidgetItem);
        listWidget->takeItem(listWidget->currentRow());
        QString abPath=subWindow->toolTip()+"/"+listWidgetItem->toolTip(); // flowpath+fileName
        QFile rmFile(abPath);  // 只是文件名而已
        if(!rmFile.remove())
        {
            qDebug()<<"QJDMdi::delFlow::remove file failed";
            return false;
        }

        delete listWidgetItem;
    }

    refreshPosFile(); // 删除完要刷新
    return true;
}

void QJDMdi::openCloseFlow(QList<QListWidgetItem *> itemList)
{
    for(int i=0;i<itemList.size();i++)
    {
        QListWidgetItem *item=itemList.at(i);
        if(item->statusTip()=="0")
        {
            item->setStatusTip("1");
            item->setBackgroundColor(QColor(255,255,255));
            item->setIcon(QIcon(":/src/images/run.png"));

        }
        else
        {
            item->setStatusTip("0");
            item->setBackgroundColor(QColor(188,188,188));
            item->setIcon(QIcon(":/src/images/norun.png"));
        }
    }
}

// 一定要在这个函数里面返回唯一的文件名
QString QJDMdi::checkFileIfExist(QString fileName)
{
    QFile checkFile(fileName);
    while(checkFile.exists()==true)
    {
        qDebug()<<"copy to file already exist. need to change the name";
        fileName=fileName+".1";  // 这个怎么传出去
        checkFile.setFileName(fileName);
    }
    return fileName;
}

bool QJDMdi::refreshPosFile()
{
//    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!refreshPosFile!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    FLOW_PATH.clear();
    ITEM_PATH_LIST.clear();
    ITEM_STATUS_LIST.clear();

    if(currentSubWindow()==NULL)
    {
        qDebug()<<"QJDMdi::refreshPosFile::current do not have any window";
        return false;
    }

    QMdiSubWindow *subWindow=currentSubWindow();
    QString flowPath=subWindow->toolTip();
    QJDMdiSubListWidget *listWidget=hashWindow_List.value(subWindow);

    QStringList itemNameList;
    QStringList itemPathList;  // 只是一个文件名而已,需要用flowPath+itemPath
    QStringList itemStatusList;
    /// 包含了所有的list,如果需要隐藏怎么办,这个需要考虑
    for(int i=0;i<listWidget->count();i++)
    {
        QListWidgetItem *listItem=listWidget->item(i);
        QString itemName=listItem->text();
        QString itemPath=listItem->toolTip();
        QString itemStatus=listItem->statusTip();
        qDebug()<<listItem<<itemName<<itemPath<<itemStatus;
        itemNameList<<itemName;
        itemPathList<<itemPath;
        itemStatusList<<itemStatus;
    }

    /// pos文件的写入应当在每次添加或删除的时候就要执行
    // 写入pos文件,重新写
    QFile posFile(flowPath+"/pos");
    qDebug()<<"QJDMdi::refreshPosFile::"<<flowPath<<posFile.fileName();
    if(!posFile.open(QFile::WriteOnly))
    {qDebug()<<"posFile open failed";}
    if(itemNameList.count()==0)
    {
        qDebug()<<"QJDMdi::refreshPosFile::nothing to write into the pos file";
        posFile.close();
        return false;
    }
    QTextStream ts(&posFile);
    for(int i=0;i<itemNameList.count();i++)
    {
        ts<<itemNameList.at(i)<<";";
    }
    ts<<"\n";
    for(int i=0;i<itemPathList.count();i++)
    {
        ts<<itemPathList.at(i)<<";";
    }
    ts<<"\n";
    for(int i=0;i<itemStatusList.count();i++)
    {
        ts<<itemStatusList.at(i)<<";";
    }

    posFile.close();

    FLOW_PATH=flowPath;
    ITEM_PATH_LIST=itemPathList;
    ITEM_STATUS_LIST=itemStatusList;

    return true;
}

void QJDMdi::excuteFlow()
{
    /// 总感觉不能依靠此来判断
    if(ITEM_PATH_LIST.count()==0)
    {
        QMessageBox::information(this,"No flow excuted","Please open a flow before you can excute it.",QMessageBox::Ok);
        return;
    }

    if(!refreshPosFile())
    {
        QMessageBox::information(this,"No flow excuted","Please open a flow before you can excute it.",QMessageBox::Ok);
        return;
    }

//    qDebug()<<"~~~~~"<<FLOW_PATH<<ITEM_PATH_LIST<<ITEM_STATUS_LIST;
    /// ----------按照顺序解析xml模块,输出至输出文件-------------- ///

    QStringList xmlFilePathList;
    for(int i=0;i<ITEM_PATH_LIST.count();i++)
    {
//        qDebug()<<"ITEM_STATUS_LIST"<<ITEM_STATUS_LIST.at(i);
        if(ITEM_STATUS_LIST.at(i)!="0")
        {
            // 开启的list才会被输入
            xmlFilePathList<<FLOW_PATH+"/"+ITEM_PATH_LIST.at(i); // 这才是绝对路径
        }
    }

    moduleNameList.clear();
    module_PropertyList.clear();
    if(!processFlowXML(xmlFilePathList))
        qDebug()<<"processFlowXML failed";

    // 输出参数文件
    QString argFileName=outputMainFlowArguFile(moduleNameList,module_PropertyList);
    // 执行
    runProcess(argFileName);
    // 这里需要弹出窗口,显示进程信息之类
}

bool QJDMdi::processFlowXML(QStringList xmlPathList)
{
    for(int i=0;i<xmlPathList.count();i++)
    {
        QFile xmlFile(xmlPathList.at(i));
        qDebug()<<"\nxml file name:"<<xmlPathList.at(i);
        if(xmlFile.open(QFile::ReadOnly)==false)
        {
            qDebug()<<"xml file open failed";
            return false;
        }

        /// 处理xml

        QDomDocument domDocument;
        domDocument.clear();
        QString errorStr;
        int errorLine;
        int errorColumn;
        if (!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine,&errorColumn))
        {
            // 错误处理，先不必要
            qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            return false;
        }
        QDomElement root = domDocument.documentElement();
        if (root.tagName() != "ui")   // tag 限定
        {
            qDebug()<<tr("The file is not an ui  file.");
            return false;
        }

        QDomElement moduleNameEle = root.firstChildElement("module");

        while (!moduleNameEle.isNull())
        {
            QString name = moduleNameEle.attribute("name");  // module name
            moduleName=name;
            moduleNameList.append(moduleName);
            parseModuleElement(moduleNameEle);
            moduleNameEle = moduleNameEle.nextSiblingElement("module");


            moduleName.clear();
            propertyNameAndValueList.clear();
//            propertyName.clear();  // may be not this place
//            propertyValue.clear();
        }

    }
//    qDebug()<<"module list::"<<moduleNameList;
//    qDebug()<<"module_PropertyList"<<module_PropertyList;
    return true;
}

QString QJDMdi::outputMainFlowArguFile(QStringList nameList, QList<QStringList> propertyList)
{
    // 统一输出位置仍然没有决定,不能输出到data下面去,应当放到用户的配置文件下面去
    // 此时不适用qsettings
    qDebug()<<"module list::"<<nameList;
    qDebug()<<"module_PropertyList"<<propertyList;
    qDebug()<<QApplication::applicationDirPath()<<QApplication::applicationFilePath(); // 区别
    QFile arguFile;
    QString fileName=QApplication::applicationDirPath()+"/opt/ui_mainflow.arg";
    arguFile.setFileName(fileName);
    if(!arguFile.open(QFile::WriteOnly))
        qDebug()<<"argu file open failed";

    QMdiSubWindow *subWindow=currentSubWindow();
    QJDMdiSubListWidget *listWidget=hashWindow_List.value(subWindow);

    QString flowPath=subWindow->toolTip();
    QString logPath=flowPath+"/log/"+listWidget->windowTitle()+"_"
            +QDateTime::currentDateTime().toString("yyMdhmsz")+".log"; /// 后续还需要加上时间好了

    //1.1
    //2.应用程序路径
    //3.日志路径
    //4.99
    //5.各个模块及参数
    QTextStream ts(&arguFile);
    ts<<"1\n";
    ts<<QApplication::applicationFilePath()+"\n";
    ts<<logPath+"\n";
    ts<<"99\n";
    for(int i=0;i<nameList.size();i++)
    {
        ts<<nameList.at(i)<<"|";
        for(int j=0;j<propertyList.at(i).size();j++)
        {
            ts<<propertyList[i].at(j)<<";";
        }
        ts<<"\n";
    }

    arguFile.close();

    return fileName;
}

void QJDMdi::runProcess(QString arg)
{
    QStringList paralist;
    QString filepath=arg;
    paralist.append(filepath);
    qDebug()<<"Para List::"<<paralist;

    /// --------------- 每执行一个，信息应当保存，直到被覆盖----------------///
    QProcess *JDP=new QProcess;
//    connect(JDP, SIGNAL(finished(int,QProcess::ExitStatus)),
//            this, SLOT(processFinished( int, QProcess::ExitStatus)));

//    JDP->start("./MainFlow",paralist);
    JDP->start("geany");
    /// --------------- 每执行一个，信息应当保存，直到被覆盖----------------///
    QJDProcessWidget *processWidget=new QJDProcessWidget;
    processWidget->addTask(QString::number(JDP->pid()));
    processWidget->show();
}

void QJDMdi::parseModuleElement(QDomElement const& moduleNameEle)
{
    QDomElement child=moduleNameEle.firstChildElement("property");

    while (!child.isNull())
    {
         // property name,多个,如何处理到一个module name之下,还要保证有重名的情况下
        QString name = child.attribute("name");
        parsePropertyElement(child);
        child = child.nextSiblingElement("property");
    }
//    qDebug()<<propertyNameAndValueList;
   module_PropertyList.append(propertyNameAndValueList);
}

/// 处理各个属性
void QJDMdi::parsePropertyElement(QDomElement const& property)
{
    // 需要考,考虑控件类型
    QDomElement displayType=property.firstChildElement("displaytype");
    QDomElement hideEle=property.firstChildElement("hide");
    if(hideEle.text()=="YES")
    {
        return;
    }

    QString widgetType=displayType.attribute("name");
    if(widgetType=="fileread" || widgetType=="filesave")
    {
//        qDebug()<<"fileread write in as:: "<<property.attribute("name")<<displayType.text();
//        textStream<<property.attribute("name")<<"="<<displayType.text()<<";";
        propertyNameAndValue=property.attribute("name")+"="+displayType.text();
        qDebug()<<propertyNameAndValue;
        propertyNameAndValueList.append(propertyNameAndValue);
        propertyNameAndValue.clear();
    }
    if(widgetType=="lineedit" || widgetType=="spinbox")
    {
//        qDebug()<<"line/spin write in as:: "<<property.attribute("name")<<displayType.text();
//        textStream<<property.attribute("name")<<"="<<displayType.text()<<";";
        propertyNameAndValue=property.attribute("name")+"="+displayType.text();
        qDebug()<<propertyNameAndValue;

        propertyNameAndValueList.append(propertyNameAndValue);
        propertyNameAndValue.clear();
    }
    if(widgetType=="checkbox")
    {
        if(displayType.text()=="checked")
        {
//            qDebug()<<"check write in as:: "<<property.attribute("name")<<"1";
//            textStream<<property.attribute("name")<<"="<<"1"<<";";
            propertyNameAndValue=property.attribute("name")+"=1";
            qDebug()<<propertyNameAndValue;

            propertyNameAndValueList.append(propertyNameAndValue);
            propertyNameAndValue.clear();
        }
        if(displayType.text()=="unchecked")
        {
//            qDebug()<<"check write in as:: "<<property.attribute("name")<<"0";
//            textStream<<property.attribute("name")<<"="<<"0"<<";";
            propertyNameAndValue=property.attribute("name")+"=0";
            qDebug()<<propertyNameAndValue;

            propertyNameAndValueList.append(propertyNameAndValue);
            propertyNameAndValue.clear();
        }
    }
    if(widgetType=="combobox")
    {
        /// 还要处理下
        qDebug()<<"widgetType is combobox";
        QDomElement option=displayType.firstChildElement("option");
        while(!option.isNull())
        {
            if(option.text()=="checked")
            {
//                qDebug()<<"combo write in as:: "<<property.attribute("name")<<option.attribute("value");
//                textStream<<property.attribute("name")<<"="<<option.attribute("value")<<";";
                propertyNameAndValue=property.attribute("name")+"="+option.attribute("value");
                qDebug()<<propertyNameAndValue;

                propertyNameAndValueList.append(propertyNameAndValue);
                propertyNameAndValue.clear();
            }
            option=option.nextSiblingElement("option");
        }
    }
    if(widgetType=="filecombobox")
    {
        /// 还要处理下
        qDebug()<<"widgetType is filecombobox";
        QDomElement option=displayType.firstChildElement("option");
        while(!option.isNull())
        {
//            qDebug()<<"combo write in as:: "<<property.attribute("name")<<option.attribute("value");
//            textStream<<property.attribute("name")<<"="<<option.attribute("value")<<";";
            propertyNameAndValue=property.attribute("name")+"="+option.attribute("value");
            qDebug()<<propertyNameAndValue;

            propertyNameAndValueList.append(propertyNameAndValue);
            propertyNameAndValue.clear();
            option=option.nextSiblingElement("option");
        }
    }
    if(widgetType=="radiobutton")
    {
        qDebug()<<"widgetType is radioButton";
        QDomElement option=displayType.firstChildElement("option");
        while(!option.isNull())
        {
            if(option.text()=="checked")
            {
//                qDebug()<<"radio write in as:: "<<property.attribute("name")<<option.attribute("value");
//                textStream<<property.attribute("name")<<"="<<option.attribute("value")<<";";
                propertyNameAndValue=property.attribute("name")+"="+option.attribute("value");
                qDebug()<<propertyNameAndValue;

                propertyNameAndValueList.append(propertyNameAndValue);
                propertyNameAndValue.clear();
            }
            option=option.nextSiblingElement("option");
        }
    }
}

void QJDMdi::showFunArgu(QString name,QString path)
{
    // 调用qjdfunarguwidget 显示
    // 下一个点击时 这个要主动关闭 但是要保留修改
    qDebug()<<name<<path; // 最新的点击名称和路径
    QJDFunArguWidget *funWidget=new QJDFunArguWidget;
    // 这里需要传进去完整路径,而不是文件名
    QMdiSubWindow *subWindow=currentSubWindow();
    QString flowPath=subWindow->toolTip();
    QString abPath=flowPath+"/"+path;
    qDebug()<<abPath;
//    funWidget->setData(name,abPath);
    if(funWidget->setData(name,abPath)==false)
    {
        // 关闭当前的subWindow
        QMessageBox::critical(this,"Error occurred!","Can`t display this funcation arguments, it maybe already deleted, the window will now be closed.",QMessageBox::Ok);
        QMdiSubWindow *subWindow=currentSubWindow();
        hashWindow_List.remove(subWindow);
        this->removeSubWindow(subWindow);
//        delete subWindow;  // 这个不能删除,否则会引起崩溃
        return;
    }
    funWidget->move(QCursor::pos());
    funWidget->show();
}

void QJDMdi::listWidgetItemMidClicked(QListWidgetItem *item)
{
    // 这个不错,万能
    qDebug()<<"listWidgetItemMidClicked::"<<item->text()<<item->toolTip();
    listWidgetItemName=item->text();
    listWidgetItemTooltip=item->toolTip();

    showFunArgu(listWidgetItemName,listWidgetItemTooltip);
}

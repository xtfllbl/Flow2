#include "qjdfunarguwidget.h"
#include <QDebug>
#include <QFile>
#include "QJD/qjdpushbutton.h"
#include "QJD/qjdfilechooseread.h"
#include "QJD/qjdfilechoosesave.h"
#include "QJD/qjdlineedit.h"
#include "QJD/qjdcombobox.h"
#include "QJD/qjdfilereadline.h"
#include "QJD/qjdfilesaveline.h"
#include "QJD/qjdcheckbox.h"
#include "QJD/qjdspinbox.h"
#include "QJD/qjdradiobutton.h"
#include "QJD/qjdbuttongroup.h"

QJDFunArguWidget::QJDFunArguWidget(QWidget *parent) :
    QWidget(parent)
{
//    hashWhatIsThisWidget.clear();
//    widgetList.clear();
    finalLayout=new QVBoxLayout();
    propertyList.clear();
    this->setWindowFlags(Qt::Window
                              | Qt::WindowCloseButtonHint);  // 单关闭按钮即可
}

bool QJDFunArguWidget::setData(QString name, QString path)
{
    qDebug()<<name<<path;
    ARGU_FILE_PATH=path;
    DATA_PATH=ARGU_FILE_PATH.left(ARGU_FILE_PATH.lastIndexOf("/"));  // 去文件名
    DATA_PATH=DATA_PATH.left(DATA_PATH.lastIndexOf("/"));   //去流程目录
    DATA_PATH=DATA_PATH+"/Data";
    qDebug()<<"~~~~~~~~~~~~~~~~"<<DATA_PATH;
    // 有了名称和路径,需要解析xml并且显示
    QStringList areaLineFlow=path.split("/",QString::SkipEmptyParts);
//    qDebug()<<"----------------"<<areaLineFlow;
    QString flowString=areaLineFlow.at(areaLineFlow.size()-2);
    this->setWindowTitle(flowString+" :: "+name);

    QFile xmlFile(path);
    if(!xmlFile.open(QFile::ReadOnly))
    {
        qDebug()<<"QJDFunArguWidget::setData::xml File open failed";
        return false;
    }

    if(!readFile(&xmlFile))
    {
        qDebug()<<"read file failed";
        return false;
    }

    xmlFile.close();
    return true;
}

bool QJDFunArguWidget::readFile(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    // 这是一切的开始，将所有数据全部读入了，所以修改之后写出就是了，非常方便
    if (!domDocument.setContent(device, true, &errorStr, &errorLine,&errorColumn))
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

    QDomElement module = root.firstChildElement("module");
    modulename=module.attribute("name");
    moduleversion=module.attribute("version");

    QDomElement child=module.firstChildElement("property");

    while (!child.isNull())
    {
        property = child.attribute("name");
        parsePropertyElement(child);
        creatWidgetItem(property,desc,datatype,displaytype,displayvalue,optionstring);

        child = child.nextSiblingElement("property");
    }

    finishCreatUI();

    return true;
}

// 只管处理,分类去其他地方
void QJDFunArguWidget::parsePropertyElement(const QDomElement &element)
{
    desc.clear();
    datatype.clear();
    displaytype.clear();
    displayvalue.clear();
    optionstring.clear();

    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
//        qDebug()<<"tagName"<<child.tagName()<<child.text();

        if (child.tagName() == "desc")
        {
            desc=child.text();
        }
        else if (child.tagName() == "datatype")
        {
            datatype=child.text();
        }
        else if (child.tagName() == "displaytype")
        {
            displaytype=child.attribute("name");
            displayvalue=child.text();

            if(child.hasAttribute("option"))
            {
                optionstring=child.attribute("option");
            }
        }

//        qDebug()<<"~~~~~"<<property<<desc<<datatype<<displaytype<<displayvalue;

        child = child.nextSiblingElement(); //循环
    }
}

void QJDFunArguWidget::creatWidgetItem(QString property, QString desc, QString datatype,
                                       QString displaytype, QString displayvalue, QString optionstring)
{
    QHBoxLayout *normalLayout=new QHBoxLayout();
    qDebug()<<"\nQJDFunArguWidget::creatWidgetItem::"
           <<property<<desc<<datatype<<displaytype<<displayvalue<<optionstring;

    QStringList argList;
    argList<<property<<desc<<datatype<<displaytype<<displayvalue<<optionstring;
    propertyList.append(argList);

    QLabel *proLabel=new QLabel;

    /// 现在不需要带id 的这些控件了,无需实时改动xml,但是改完了还是要改xml的,如何返回信息呢
    QJDLineEdit *lineEdit=new QJDLineEdit;
    QJDCheckBox *checkBox=new QJDCheckBox;
    QJDComboBox *comboBox=new QJDComboBox;
    QJDSpinBox *spinBox=new QJDSpinBox;
    QJDButtonGroup *buttonGroup=new QJDButtonGroup;
//    QJDRadioButton *radioButton=new QJDRadioButton;
    // self edit
    QJDFileReadLine *fileRead=new QJDFileReadLine;
    QJDFileSaveLine *fileSave=new QJDFileSaveLine;
    QJDFileChooseRead *fileChooseRead=new QJDFileChooseRead;
    QJDFileChooseSave *fileChooseSave=new QJDFileChooseSave;

//    QList<QString> radioIDList;

    if(!property.isEmpty())   // 名称，作为label
    {
        if(displaytype!="fileChooseRead")
        {
            proLabel->setText(property);
            proLabel->setFixedWidth(150); // 现在可以有一个较大的弹出窗口
            proLabel->setFrameShape(QFrame::Panel);
            proLabel->setFrameShadow(QFrame::Raised);
            normalLayout->addWidget(proLabel);
        }
    }
    if(!desc.isEmpty())  //描述，tooltip
    {
        proLabel->setToolTip(desc);
    }

    /// --------------------- 在此，需要和writejob联系起来，动态更改xml------------------ ///
    if(!displaytype.isEmpty())    // 创建控件的类型
    {
        /// ------------------------------------lineEdit-------------------------------//
        /// 需要值 lineEdit.text,只需要返回这个即可
        if(displaytype=="lineedit")
        {
            lineEdit->setPropertyList(argList);
            lineEdit->setMinimumWidth(200);
            lineEdit->setText(displayvalue);
            lineEdit->propertyInt=propertyList.size()-1;

            connect(lineEdit,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(lineEdit);
        }

        /// ------------------------------------fileReadLine-------------------------------//
        if(displaytype=="fileread")
        {
            fileRead->setPropertyList(argList);
            fileRead->setMinimumWidth(200);
            fileRead->setText(displayvalue);
            fileRead->propertyInt=propertyList.size()-1;

            connect(fileRead,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileRead);

        }

        /// ------------------------------------fileSaveLine-------------------------------//
        if(displaytype=="filesave")
        {
            fileSave->setPropertyList(argList);
            fileSave->setMinimumWidth(200);
            fileSave->setText(displayvalue);
            fileSave->propertyInt=propertyList.size()-1;

            connect(fileSave,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileSave);
        }

        /// ------------------------------------fileChooseRead-------------------------------//
        if(displaytype=="filechooseread")
        {
            fileChooseRead->setPropertyList(argList);
            fileChooseRead->propertyInt=propertyList.size()-1;
            fileChooseRead->setTypeData(DATA_PATH);

            connect(fileChooseRead,SIGNAL(sigFileChooseReadEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileChooseRead);
        }

        /// ------------------------------------fileChooseSave-------------------------------//
        if(displaytype=="filechoosesave")
        {
            QStringList optionList;
            optionList=optionstring.split(";",QString::SkipEmptyParts);

            fileChooseSave->setPropertyList(argList);
//            fileChooseSave->setMinimumWidth(200);
            for(int i=0;i<optionList.size();i++)
            {
                fileChooseSave->addComboBoxItem(optionList.at(i));
                fileChooseSave->setComboBoxCurrentIndex(0);
            }
            QFileInfo finfo(displayvalue);
            fileChooseSave->setLineText(finfo.fileName());
            fileChooseSave->setDataPath(DATA_PATH);
            fileChooseSave->propertyInt=propertyList.size()-1;

            connect(fileChooseSave,SIGNAL(sigFileChooseSaveEditChanged(int,QStringList)),
                    this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileChooseSave);
        }
        /// ------------------------------------checkBox-------------------------------//
        if(displaytype=="checkbox")
        {
            checkBox->setPropertyList(argList);
            checkBox->setMinimumWidth(200);
            if(displayvalue=="checked")
            {
                checkBox->setChecked(true);
            }
            checkBox->propertyInt=propertyList.size()-1;

            connect(checkBox,SIGNAL(sigCheckBoxChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(checkBox);
        }

        /// ------------------------------------spinbox-------------------------------//
        if(displaytype=="spinbox")
        {
            spinBox->setPropertyList(argList);
            spinBox->setMinimumWidth(200);
            spinBox->setValue(displayvalue.toInt());
            spinBox->propertyInt=propertyList.size()-1;

            connect(spinBox,SIGNAL(sigValueChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(spinBox);
        }

        /// ------------------------------------combobox-------------------------------//
        /// 和默认的不一样,需要多一个option stringlist
        if(displaytype=="combobox")
        {
            QStringList optionList;
            optionList=optionstring.split(";",QString::SkipEmptyParts);

            comboBox->setPropertyList(argList);
            comboBox->setMinimumWidth(200);
            for(int i=0;i<optionList.size();i++)
            {
                comboBox->addItem(optionList.at(i));
                if(optionList.at(i)==displayvalue)
                {
                    comboBox->setCurrentIndex(i);
                }
            }
            comboBox->propertyInt=propertyList.size()-1;

            connect(comboBox,SIGNAL(sigComboBoxChanged(int,QStringList)),
                    this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(comboBox);
        }

        /// ------------------------------------radiobutton-------------------------------//
        /// 需要生成多个radiobutton,如何处理
        if(displaytype=="radiobutton")
        {
            QStringList optionList;
            optionList=optionstring.split(";",QString::SkipEmptyParts);
            QHBoxLayout *radioLayout=new QHBoxLayout();

            buttonGroup->setPropertyList(argList);
            for(int i=0;i<optionList.size();i++)
            {
                QRadioButton *radioButton=new QRadioButton;
                radioButton->setText(optionList.at(i));
                radioButton->setMinimumWidth(200);

                buttonGroup->addButton(radioButton);
                radioLayout->addWidget(radioButton);

                if(optionList.at(i)==displayvalue)
                {
                    radioButton->setChecked(true);
                }
            }
            buttonGroup->propertyInt=propertyList.size()-1;  //应当是一个什么包含在里面,而用那个来设置

            connect(buttonGroup,SIGNAL(sigButtonGroupChecked(int,QStringList)),
                    this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addLayout(radioLayout);
        }
    }
    finalLayout->addLayout(normalLayout);
}

void QJDFunArguWidget::finishCreatUI()
{
    /// 结束前添加spacer
    QHBoxLayout *hLayout=new QHBoxLayout;
    QPushButton *saveButton=new QPushButton;
    saveButton->setText("Save");
//    saveButton->setIcon(QIcon(":/src/images/save.png")); // 图标较突兀
    QPushButton *cancleButton=new QPushButton;
    cancleButton->setText("Cancle");
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveArgToXml()));
    connect(cancleButton,SIGNAL(clicked()),this,SLOT(close()));

    hLayout->addWidget(saveButton);
    hLayout->addWidget(cancleButton);
    finalLayout->addLayout(hLayout);
    QSpacerItem *verticalSpacer = new QSpacerItem(148, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    finalLayout->addItem(verticalSpacer);
    setLayout(finalLayout);
}

// 貌似propertyName不需要
void QJDFunArguWidget::changeDisplayValue(int propertyListInt ,QStringList changeList)
{
    qDebug()<<"changeDisplayValue::"<<propertyList.at(propertyListInt)<<changeList;
    propertyList.replace(propertyListInt,changeList);
//    qDebug()<<propertyList;
    emit sigHighlightSave();
}

void QJDFunArguWidget::saveArgToXml()
{
    qDebug()<<"saveArgToXml::"<<ARGU_FILE_PATH;
    qDebug()<<propertyList;
    /// 将propertyList当中的东西重新写回去
    QFile xmlFile(ARGU_FILE_PATH);
    if(!xmlFile.open(QFile::WriteOnly))
        qDebug()<<"QJDFunArguWidget::saveArgToXml::xml File open failed";

    // 1. 写开头
    QTextStream out(&xmlFile);
    out<<"<?xml version='1.0' encoding='UTF-8'?>\n"
            <<"<!DOCTYPE ui>\n"
            <<"<ui version=\"1.0\">\n"
           <<"</ui>\n";
    xmlFile.close();

    // 2. 添加内容
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text))
        qDebug()<<"QJDFunArguWidget::saveArgToXml::xml File open failed";
    if(!prepareWriteFile(&xmlFile))
        qDebug()<<"prepareWriteFile failed";
    xmlFile.close();

    // 3.正式写入
    if(!write())
        qDebug()<<"writejob failed";

    this->close();
}

bool QJDFunArguWidget::prepareWriteFile(QIODevice *file)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(file, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<"has sth wrong,need init first";
        QWidget *widgetMSG=new QWidget();
        QMessageBox::information(widgetMSG, tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        file->close();
        return false;
    }
    QDomElement root = doc.documentElement();
    qDebug()<<root.text();
    QDomElement moduleEle=doc.createElement("module");
    root.appendChild(moduleEle);  /// 应当在模块内部添加，而不是外部

    // module
    QDomAttr moduleVersionAttr=doc.createAttribute("version");
    moduleVersionAttr.setValue(moduleversion);
    moduleEle.setAttributeNode(moduleVersionAttr);

    QDomAttr moduleNameAttr=doc.createAttribute("name");
    moduleNameAttr.setValue(modulename);
    moduleEle.setAttributeNode(moduleNameAttr);

    // property desc datatype displaytype displayvalue option
    for(int i=0;i<propertyList.size();i++)
    {
        QDomElement propertyEle =  doc.createElement("property");
        QDomElement descEle =  doc.createElement("desc");
        QDomElement datatypeEle =  doc.createElement("datatype");
        QDomElement displaytypeEle =  doc.createElement("displaytype");

        // property
        QDomAttr propertyAttr=doc.createAttribute("name");
        propertyAttr.setValue(propertyList[i].at(0));
        propertyEle.setAttributeNode(propertyAttr);
        moduleEle.appendChild(propertyEle);

        // desc
        QDomText descText=doc.createTextNode(propertyList[i].at(1));
        descEle.appendChild(descText);
        propertyEle.appendChild(descEle);

        // datatype
        QDomText datatypeText=doc.createTextNode(propertyList[i].at(2));
        datatypeEle.appendChild(datatypeText);
        propertyEle.appendChild(datatypeEle);

        // displaytype
        if(!propertyList[i].at(5).isEmpty())
        {
            QDomAttr displaytypeOptionAttr=doc.createAttribute("option");
            displaytypeOptionAttr.setValue(propertyList[i].at(5));
            displaytypeEle.setAttributeNode(displaytypeOptionAttr);
        }
        QDomAttr displaytypeNameAttr=doc.createAttribute("name");
        displaytypeNameAttr.setValue(propertyList[i].at(3));
        displaytypeEle.setAttributeNode(displaytypeNameAttr);
        QDomText displayvalueText=doc.createTextNode(propertyList[i].at(4));
        displaytypeEle.appendChild(displayvalueText);
        propertyEle.appendChild(displaytypeEle);
    }

    return true;
}

bool QJDFunArguWidget::write()
{
    qDebug()<<"QJDFunArguWidget::write xml";
    /// 写可以等模块分析完毕之后
    QFile fileWrite;
    fileWrite.setFileName(ARGU_FILE_PATH);
    if(!fileWrite.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"can`t open jobFileXML in writeOnly";
        return false;
    }

    const int IndentSize = 4;
    QTextStream out(&fileWrite);
    doc.save(out, IndentSize);   //doc是可以延续的
    fileWrite.close();

    return true;
}

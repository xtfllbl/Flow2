#include "qjdfunarguwidget.h"
#include <QDebug>
#include <QFile>
#include "QJD/qjdlineedit.h"
#include "QJD/qjdfilecombobox.h"
#include "QJD/qjdfilereadline.h"
#include "QJD/qjdfilesaveline.h"
#include "QJD/qjdcheckbox.h"
#include "QJD/qjdspinbox.h"

QJDFunArguWidget::QJDFunArguWidget(QWidget *parent) :
    QWidget(parent)
{
//    hashWhatIsThisWidget.clear();
//    widgetList.clear();
    finalLayout=new QVBoxLayout();
    propertyList.clear();
}

void QJDFunArguWidget::setData(QString name, QString path)
{
    qDebug()<<name<<path;
    // 有了名称和路径,需要解析xml并且显示
    this->setWindowTitle(name);

    QFile xmlFile(path);
    if(!xmlFile.open(QFile::ReadOnly))
        qDebug()<<"QJDFunArguWidget::setData::xml File open failed";

    if(!readFile(&xmlFile))
        qDebug()<<"read file failed";
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

    QDomElement child=module.firstChildElement("property");

    while (!child.isNull())
    {
//        nodeClear();  //清理节点包含信息
        property = child.attribute("name");

        parsePropertyElement(child);
        // 设置一套机制传送信息
        /// 貌似有没有没有必要,分类过于复杂,需要更换处理方式
//        emit sigNodeData(property,desc,datatype,min,max,displaytype,displaytext,displayvalue,
//                         optiontext,optionvalue,hidevalue,hidetype);

        creatWidgetItem(property,desc,datatype,displaytype,displayvalue);
        child = child.nextSiblingElement("property");
    }

    finishCreatUI();
}

// 只管处理,分类去其他地方
void QJDFunArguWidget::parsePropertyElement(const QDomElement &element)
{
    desc.clear();
    datatype.clear();
    displaytype.clear();
    displayvalue.clear();

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

//            QDomElement option;
//            if(child.hasChildNodes())   /// option 没了?
//            {
//                option=child.firstChildElement("option");
//                // 遍历所有节点, 还需要知道节点的value
//                while(!option.isNull())
//                {
//                    optionvalue<<option.text();
//                    optiontext<<option.attribute("value");
//                    option=option.nextSiblingElement();
//                }
//            }
        }

//        qDebug()<<"~~~~~"<<property<<desc<<datatype<<displaytype<<displayvalue;

        child = child.nextSiblingElement(); //循环
    }
}

void QJDFunArguWidget::creatWidgetItem(QString property, QString desc, QString datatype,
                                       QString displaytype, QString displayvalue)
{
    QHBoxLayout *normalLayout=new QHBoxLayout();
    qDebug()<<"\nQJDFunArguWidget::creatWidgetItem::"<<property<<desc<<datatype<<displaytype<<displayvalue;
    QStringList arg;
    arg<<property<<desc<<datatype<<displaytype<<displayvalue;
    propertyList.append(arg);

    QLabel *proLabel=new QLabel;

    /// 现在不需要带id 的这些控件了,无需实时改动xml,但是改完了还是要改xml的,如何返回信息呢
    QJDLineEdit *lineEdit=new QJDLineEdit;
    QJDCheckBox *checkBox=new QJDCheckBox;
    QComboBox *comboBox=new QComboBox;
    QJDSpinBox *spinBox=new QJDSpinBox;
    QGroupBox *groupBox=new QGroupBox;
    // self edit
    QJDFileReadLine *fileRead=new QJDFileReadLine;
    QJDFileSaveLine *fileSave=new QJDFileSaveLine;
    QJDFileComboBox *fileComboBox=new QJDFileComboBox;

    QList<QString> radioIDList;

    if(!property.isEmpty())   // 名称，作为label
    {
        proLabel->setText(property);
        proLabel->setFixedWidth(150); // 现在可以有一个较大的弹出窗口
        proLabel->setFrameShape(QFrame::Panel);
        proLabel->setFrameShadow(QFrame::Raised);
        normalLayout->addWidget(proLabel);
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
            lineEdit->setPropertyList(property,desc,datatype,displaytype);
            lineEdit->setMinimumWidth(200);
            lineEdit->setText(displayvalue);
            lineEdit->propertyInt=propertyList.size()-1;

            connect(lineEdit,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(lineEdit);
        }

        /// ------------------------------------fileReadLine-------------------------------//
        if(displaytype=="fileread")
        {
            fileRead->setPropertyList(property,desc,datatype,displaytype);
            fileRead->setMinimumWidth(200);
            fileRead->setText(displayvalue);
            fileRead->propertyInt=propertyList.size()-1;

            connect(fileRead,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileRead);

        }

        /// ------------------------------------fileSaveLine-------------------------------//
        if(displaytype=="filesave")
        {
            fileSave->setPropertyList(property,desc,datatype,displaytype);
            fileSave->setMinimumWidth(200);
            fileSave->setText(displayvalue);
            fileSave->propertyInt=propertyList.size()-1;

            connect(fileSave,SIGNAL(sigLineEditChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(fileSave);
        }

        /// ------------------------------------checkBox-------------------------------//
        if(displaytype=="checkbox")
        {
            checkBox->setPropertyList(property,desc,datatype,displaytype);
            checkBox->setMinimumWidth(200);
            if(displayvalue=="checked")
            {
                checkBox->setChecked(true);
            }
            checkBox->propertyInt=propertyList.size()-1;

            connect(checkBox,SIGNAL(sigCheckBoxChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(checkBox);
        }

        /// ------------------------------------combobox-------------------------------//
//        if(displaytype=="combobox")
//        {
//            comboBox->setMinimumWidth(200);
//            for(int i=0;i<optionvalue.size();i++)
//            {
//                comboBox->addItem(optiontext[i]);
//            }
//            normalLayout->addWidget(comboBox);
//        }
        /// ------------------------------------filecombobox-------------------------------//
//        if(displaytype=="filecombobox")
//        {
//            fileComboBox->setMinimumWidth(200);
//            //遍历optionvalue，然后添加到comboBox当中去
//            QDir dir;
//            dir.setPath(optionvalue.at(0));
//            dir.setFilter(QDir::Files | QDir::NoSymLinks);
////            dir.setSorting(QDir::Size | QDir::Reversed);

//            QFileInfoList list = dir.entryInfoList();
//            fileComboBox->addItem("Please Choose");
//            for (int i = 0; i < list.size(); ++i)
//            {
//                QFileInfo fileInfo = list.at(i);
//                fileComboBox->addItem(fileInfo.fileName());
//            }

//            normalLayout->addWidget(fileComboBox);
//            widgetList.append(fileComboBox);

//            hashWhatIsThisWidget.insert(fileComboBox,"QJDFileComboBox");
//        }

        /// ------------------------------------spinbox-------------------------------//
        if(displaytype=="spinbox")
        {
            spinBox->setPropertyList(property,desc,datatype,displaytype);
            spinBox->setMinimumWidth(200);
            spinBox->setValue(displayvalue.toInt());
            spinBox->propertyInt=propertyList.size()-1;

            connect(spinBox,SIGNAL(sigValueChanged(int,QStringList)),this,SLOT(changeDisplayValue(int,QStringList)));
            normalLayout->addWidget(spinBox);
        }

        /// ------------------------------------radiobutton-------------------------------//
        /// 问题最多，最后处理
//        if(displaytype=="radiobutton")
//        {
//            // 这可是有列表和选中与否的，还要设好buttongroup等
//            QGridLayout *radioLayout=new QGridLayout();
//            QButtonGroup *group=new QButtonGroup();
//            for(int i=0;i<optiontext.size();i++)
//            {
//                radio=new QJDRadioButton();  // 因为需要很多，所以一个是不够的
//                radio->setText(optiontext.at(i));

//                if(optionvalue[i]=="checked")
//                {
//                    radio->setChecked(true);
//                }

//                /// 应该还要加上groupBox的id,另外增加
//                group->addButton(radio);
//                /// 合理分配布局
//                int a=0,b=1;
//                if(i%2==0)
//                {
//                    radioLayout->addWidget(radio,i-a,0);
//                    a++;
//                }
//                if(i%2==1)
//                {
//                    radioLayout->addWidget(radio,i-b,1);
//                    b++;
//                }

//                widgetList.append(radio);
//            }
//            groupBox->setLayout(radioLayout);
//            normalLayout->addWidget(groupBox);

//            // 这个还不好确定,因为是groupbox,所以无法得知内部内容
//            ///            hashWhatIsThisWidget.insert(spinBox,"QSpinBox");
//        }
    }
    finalLayout->addLayout(normalLayout);
}

void QJDFunArguWidget::finishCreatUI()
{
    /// 结束前添加spacer
    QHBoxLayout *hLayout=new QHBoxLayout;
    QPushButton *saveButton=new QPushButton;
    saveButton->setText("Save");
    QPushButton *cancleButton=new QPushButton;
    cancleButton->setText("Cancle");
    connect(saveButton,SIGNAL(pressed()),this,SLOT(saveArgToXml()));
    connect(cancleButton,SIGNAL(pressed()),this,SLOT(close()));

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
    qDebug()<<propertyList;
}

void QJDFunArguWidget::saveArgToXml()
{
    qDebug()<<"saveArgToXml";
    qDebug()<<propertyList;
    /// 将propertyList当中的东西重新写回去
}

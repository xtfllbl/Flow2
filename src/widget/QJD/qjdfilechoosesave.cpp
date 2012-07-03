#include "qjdfilechoosesave.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>
#include <QDebug>

QJDFileChooseSave::QJDFileChooseSave(QWidget *parent) :
    QWidget(parent)
{    
    propertyInt=-1;

    comboBox=new QComboBox;
    lineEdit=new QLineEdit;
    upLabel=new QLabel;
    downLabel=new QLabel;
    upLabel->setText("Choose Data Type:");
    downLabel->setText("File Name:");

    QGridLayout *vLayout=new QGridLayout;
    vLayout->addWidget(upLabel,0,0);
    vLayout->addWidget(comboBox,0,1);
    vLayout->addWidget(downLabel,1,0);
    vLayout->addWidget(lineEdit,1,1);

    this->setLayout(vLayout);

    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(emitEditFinished()));
}

void QJDFileChooseSave::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}


void QJDFileChooseSave::emitEditFinished()
{
//    qDebug()<<"QJDFileChooseRead::emitEditFinished";
    // 自动加了路径了,下一次打开的时候呢?
    QString value;
    if(!lineEdit->text().contains("/"))
    {
        value=DATA_PATH+"/"+comboBox->currentText()+"/"+lineEdit->text();
    }
    else
        value=lineEdit->text();

    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<value<<OPTION;
    emit sigFileChooseSaveEditChanged(propertyInt,list);
}

void QJDFileChooseSave::addComboBoxItem(QString name)
{
    comboBox->addItem(name);
}

void QJDFileChooseSave::setComboBoxCurrentIndex(int index)
{
    comboBox->setCurrentIndex(index);
}

void QJDFileChooseSave::setLineText(QString text)
{
    lineEdit->setText(text);
}

void QJDFileChooseSave::setDataPath(QString path)
{
    DATA_PATH=path;
}

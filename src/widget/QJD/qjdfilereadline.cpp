#include "qjdfilereadline.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>
#include <QDebug>

QJDFileReadLine::QJDFileReadLine(QWidget *parent) :
    QWidget(parent)
{    
    propertyInt=-1;

    lineEdit=new QLineEdit;
    fileButton=new QToolButton;
    fileButton->setText("...");
    fileButton->setIcon(QIcon(":/src/images/dir.png"));
    connect(fileButton,SIGNAL(pressed()),this,SLOT(fileButtonPressed()));

    fileButton->setMaximumHeight(lineEdit->height());

//    qDebug()<<"height::"<<fileButton->height()<<lineEdit->height();
    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(fileButton);
    this->setLayout(layout);

    /// editing finished 应当要改变, 否则通过setText不算改变
    connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

void  QJDFileReadLine::fileButtonPressed()
{
    QString _a=QFileDialog::getOpenFileName(this, tr("Choose File"), "/home", tr("Any Files (*)"));
    if(_a!="")
    {
        fileName=_a;
        lineEdit->setText(fileName);
        emitEditFinished();
    }
}

void QJDFileReadLine::emitEditFinished()
{
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<this->text()<<OPTION;
    emit sigLineEditChanged(propertyInt,list);
}

void QJDFileReadLine::setText(QString text)
{
    lineEdit->setText(text);
}

QString QJDFileReadLine::text()
{
    return lineEdit->text();
}

void QJDFileReadLine::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

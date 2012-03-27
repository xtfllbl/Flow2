#include "qjdfilereadline.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>
#include <QDebug>

QJDFileReadLine::QJDFileReadLine(QWidget *parent) :
    QWidget(parent)
{    
    lineEdit=new QLineEdit;
    fileButton=new QToolButton;
    fileButton->setText("...");
    fileButton->setIcon(QIcon(":/images/filesave.png"));
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
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<this->text();
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

void QJDFileReadLine::setPropertyList(QString propertyName, QString desc, QString dataType, QString displayType)
{
    PROPERTY=propertyName;
    DESC=desc;
    DATATYPE=dataType;
    DISPLAYTYPE=displayType;
}

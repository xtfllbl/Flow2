#include "qjdradiobutton.h"
#include <QTime>
#include <QDebug>

QJDRadioButton::QJDRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    propertyInt=-1;

    connect(this,SIGNAL(clicked(bool)),this,SLOT(emitStateChanged(bool)));

}


void QJDRadioButton::emitStateChanged(bool checked)
{
//    if(checked==true)
//    {
//        qDebug()<<"checked"<<this->id();
//        emit sigRadioChecked("checked",this->id(),otherID);   /// 如何获得groupbox的信号呢？
//    }
}

void QJDRadioButton::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

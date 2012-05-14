#include "qjdspinbox.h"
#include <QTime>
#include <QDebug>

QJDSpinBox::QJDSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    setRange(-100000,100000);
    connect(this,SIGNAL(valueChanged(QString)),this,SLOT(emitValueChanged(QString)));
    propertyInt=-1;

}

void QJDSpinBox::emitValueChanged(QString value)
{
//    qDebug()<<"QJDSpinBox::emitValueChanged"<<value;
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<value<<OPTION;
    emit sigValueChanged(propertyInt,list);
}

void QJDSpinBox::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

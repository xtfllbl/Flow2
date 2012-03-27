#include "qjdspinbox.h"
#include <QTime>
#include <QDebug>

QJDSpinBox::QJDSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    setRange(-100000,100000);
    connect(this,SIGNAL(valueChanged(QString)),this,SLOT(emitValueChanged(QString)));

}

void QJDSpinBox::emitValueChanged(QString value)
{
//    qDebug()<<"QJDSpinBox::emitValueChanged"<<value;
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<value;
    emit sigValueChanged(propertyInt,list);
}

void QJDSpinBox::setPropertyList(QString propertyName, QString desc, QString dataType, QString displayType)
{
    PROPERTY=propertyName;
    DESC=desc;
    DATATYPE=dataType;
    DISPLAYTYPE=displayType;
}

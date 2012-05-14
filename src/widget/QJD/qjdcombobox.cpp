#include "qjdcombobox.h"
#include <QTime>

QJDComboBox::QJDComboBox(QWidget *parent) :
    QComboBox(parent)
{
    propertyInt=-1;
    connect(this,SIGNAL(currentIndexChanged(QString)),this,SLOT(emitIndexChanged(QString)));
}


void QJDComboBox::emitIndexChanged(QString comboText)
{
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<comboText<<OPTION;
    emit sigComboBoxChanged(propertyInt,list);
}

void QJDComboBox::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

#include "qjdbuttongroup.h"

QJDButtonGroup::QJDButtonGroup(QObject *parent) :
    QButtonGroup(parent)
{
    propertyInt=-1;

    connect(this,SIGNAL(buttonPressed(QAbstractButton*)),this,SLOT(emitEditFinished(QAbstractButton*)));
}

void QJDButtonGroup::emitEditFinished(QAbstractButton *button)
{
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<button->text()<<OPTION;

    emit sigButtonGroupChecked(propertyInt,list);
}

void QJDButtonGroup::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

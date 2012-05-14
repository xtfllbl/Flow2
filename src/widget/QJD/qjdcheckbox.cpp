#include "qjdcheckbox.h"

QJDCheckBox::QJDCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    propertyInt=-1;

    connect(this,SIGNAL(stateChanged(int)),this,SLOT(emitEditFinished(int)));
}

void QJDCheckBox::emitEditFinished(int state)
{
    QStringList list;
    if(state==0)
    {
        list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<"unchecked"<<OPTION;
    }
    if(state==2)
    {
        list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<"checked"<<OPTION;
    }
    emit sigCheckBoxChanged(propertyInt,list);
}

void QJDCheckBox::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

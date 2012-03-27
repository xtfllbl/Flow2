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
        list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<"unchecked";
    }
    if(state==2)
    {
        list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<"checked";
    }
    emit sigCheckBoxChanged(propertyInt,list);
}

void QJDCheckBox::setPropertyList(QString propertyName, QString desc, QString dataType, QString displayType)
{
    PROPERTY=propertyName;
    DESC=desc;
    DATATYPE=dataType;
    DISPLAYTYPE=displayType;
}

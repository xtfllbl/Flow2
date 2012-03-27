#include "qjdlineedit.h"
#include <QTime>
#include <QDebug>
QJDLineEdit::QJDLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    propertyInt=-1;

    connect(this,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

void QJDLineEdit::emitEditFinished()
{
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<this->text();
    emit sigLineEditChanged(propertyInt,list);
}

void QJDLineEdit::setPropertyList(QString propertyName, QString desc, QString dataType, QString displayType)
{
    PROPERTY=propertyName;
    DESC=desc;
    DATATYPE=dataType;
    DISPLAYTYPE=displayType;
}

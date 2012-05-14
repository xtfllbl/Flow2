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
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<this->text()<<OPTION;
    emit sigLineEditChanged(propertyInt,list);
}

void QJDLineEdit::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

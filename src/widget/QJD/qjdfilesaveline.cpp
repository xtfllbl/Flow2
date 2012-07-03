#include "qjdfilesaveline.h"
#include <QLayout>
#include <QFileDialog>
#include <QTime>

QJDFileSaveLine::QJDFileSaveLine(QWidget *parent) :
    QWidget(parent)
{
    propertyInt=-1;

    lineEdit=new QLineEdit;
    fileButton=new QToolButton;
    fileButton->setText("...");
    fileButton->setIcon(QIcon(":/src/images/save.png"));
    connect(fileButton,SIGNAL(pressed()),this,SLOT(fileButtonPressed()));

    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(fileButton);
    this->setLayout(layout);

    connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(emitEditFinished()));
}

void  QJDFileSaveLine::fileButtonPressed()
{
    QString _a=QFileDialog::getSaveFileName(this, tr("Save To File "), "/home", tr("Any Files (*)"));
    if(_a!="")
    {
        fileName=_a;
        lineEdit->setText(fileName);
        emitEditFinished();
    }
}

void QJDFileSaveLine::emitEditFinished()
{
    QStringList list;
    list<<PROPERTY<<DESC<<DATATYPE<<DISPLAYTYPE<<this->text()<<OPTION;
    emit sigLineEditChanged(propertyInt,list);
}

void QJDFileSaveLine::setText(QString text)
{
    lineEdit->setText(text);
}

QString QJDFileSaveLine::text()
{
    return lineEdit->text();
}

void QJDFileSaveLine::setPropertyList(QStringList argList)
{
    PROPERTY=argList.at(0);
    DESC=argList.at(1);
    DATATYPE=argList.at(2);
    DISPLAYTYPE=argList.at(3);
    OPTION=argList.at(5);
}

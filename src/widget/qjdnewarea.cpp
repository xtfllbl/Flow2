#include "qjdnewarea.h"
#include <QLayout>
QJDNewArea::QJDNewArea(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QIcon(":/src/images/area.png"));
    applyBtn=new QPushButton;
    applyBtn->setText("Apply");
    cancleBtn=new QPushButton;
    cancleBtn->setText("Cancle");
    label=new QLabel;
    label->setText("New Area Name:");
    lineEdit=new QLineEdit;
    spacerLeft=new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    spacerRight=new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *hLayout1=new QHBoxLayout;
    QHBoxLayout *hLayout2=new QHBoxLayout;
    QVBoxLayout *vLayout=new QVBoxLayout;

    hLayout1->addWidget(label);
    hLayout1->addWidget(lineEdit);

    hLayout2->addItem(spacerLeft);
    hLayout2->addWidget(applyBtn);
    hLayout2->addWidget(cancleBtn);
    hLayout2->addItem(spacerRight);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    this->setLayout(vLayout);

    this->setWindowTitle("Creat New Area");

    connect(applyBtn,SIGNAL(clicked()),this,SLOT(apply()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancle()));
}

void QJDNewArea::apply()
{
    setAreaName();
}

void QJDNewArea::cancle()
{
    reject();
}

void QJDNewArea::setAreaName()
{
    areaName=lineEdit->text();

    if(areaName!="")
    {
        emit sigAreaName(areaName);
        accept();
    }
    else
    {
        reject();
    }
}

QString QJDNewArea::getAreaName()
{
    return areaName;
}

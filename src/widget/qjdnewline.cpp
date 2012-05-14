#include "qjdnewline.h"
#include <QLayout>
QJDNewLine::QJDNewLine(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QIcon(":/src/images/line.png"));

    applyBtn=new QPushButton;
    applyBtn->setText("Apply");
    cancleBtn=new QPushButton;
    cancleBtn->setText("Cancle");
    label=new QLabel;
    label->setText("New Line Name:");
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

    this->setWindowTitle("Creat New Line");

    connect(applyBtn,SIGNAL(clicked()),this,SLOT(apply()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancle()));
}

void QJDNewLine::apply()
{
    setLineName();
}

void QJDNewLine::cancle()
{
    reject();
}

void QJDNewLine::setLineName()
{
    lineName=lineEdit->text();

    if(lineName!="")
    {
        emit sigLineName(lineName);
        accept();
    }
    else
    {
        reject();
    }
}

QString QJDNewLine::getLineName()
{
    return lineName;
}

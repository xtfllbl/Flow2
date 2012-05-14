#include "qjdnewflow.h"
#include <QLayout>
QJDNewFlow::QJDNewFlow(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QIcon(":/src/images/flow.png"));

    applyBtn=new QPushButton;
    applyBtn->setText("Apply");
    cancleBtn=new QPushButton;
    cancleBtn->setText("Cancle");
    label=new QLabel;
    label->setText("New Flow Name:");
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

    this->setWindowTitle("Creat New Flow");

    connect(applyBtn,SIGNAL(clicked()),this,SLOT(apply()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancle()));
}

void QJDNewFlow::apply()
{
    setFlowName();
}

void QJDNewFlow::cancle()
{
    reject();
}

void QJDNewFlow::setFlowName()
{
    flowName=lineEdit->text();

    if(flowName!="")
    {
        emit sigFlowName(flowName);
        accept();
    }
    else
    {
        reject();
    }
}

QString QJDNewFlow::getFlowName()
{
    return flowName;
}

#include "qjdpushbutton.h"
#include <QDebug>
QJDPushButton::QJDPushButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QJDPushButton::enterEvent(QEvent *)
{
//    qDebug()<<"enterEvent";  //为什么总是foucusin
    this->setFlat(false);
}

void QJDPushButton::leaveEvent ( QEvent * )
{
//    qDebug()<<"leaveEvent";

    this->setFlat(true);

}

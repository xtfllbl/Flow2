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

void QJDPushButton::setHighlight()
{
    this->setStyleSheet("QPushButton { border-image: url(:/src/images/normalButton_focus.png) 5 8 5 8 stretch; border-width: 5px 8px 5px 8px; } QPushButton:hover { border-image: url(:/src/images/normalButton_hover.png) 5 8 5 8 stretch;border-width: 5px 8px 5px 8px; } QPushButton:pressed { border-image: url(:/src/images/normalButton_pressed.png) 5 8 5 8 stretch;border-width: 5px 8px 5px 8px;} ");
}

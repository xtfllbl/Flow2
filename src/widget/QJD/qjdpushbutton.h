#ifndef QJDPUSHBUTTON_H
#define QJDPUSHBUTTON_H

#include <QPushButton>

class QJDPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QJDPushButton(QWidget *parent = 0);
    
signals:
    
public slots:
    void setHighlight();
private:
    void enterEvent(QEvent *);
    void leaveEvent ( QEvent * ) ;
    
};

#endif // QJDPUSHBUTTON_H

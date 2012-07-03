#ifndef QJDFUNCATIONHEADWIDGET_H
#define QJDFUNCATIONHEADWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <widget/QJD/qjdpushbutton.h>
#include <QSpacerItem>
class QJDFuncationHeadWidget : public QFrame
{
    Q_OBJECT
public:
    explicit QJDFuncationHeadWidget(QFrame *parent = 0);
    
signals:
    void sigExpandClicked(int);
    void sigCollapseClicked();
public slots:
private:
    QLabel *textLabel;
    QJDPushButton *expandButton;
    QSpacerItem *horizontalSpacer;
private slots:
    void emitSigExpand(bool);
};

#endif // QJDFUNCATIONHEADWIDGET_H

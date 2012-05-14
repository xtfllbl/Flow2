#ifndef QJDAREAHEADWIDGET_H
#define QJDAREAHEADWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <widget/QJD/qjdpushbutton.h>
#include <QSpacerItem>

class QJDAreaHeadWidget : public QFrame
{
    Q_OBJECT
public:
    explicit QJDAreaHeadWidget(QFrame *parent = 0);
    
signals:
    void sigRefreshClicked();
    void sigExpandClicked();
    void sigCollapseClicked();
public slots:

private:
    QLabel *textLabel;
    QJDPushButton *refreshButton;
    QJDPushButton *expandButton;
    QJDPushButton *collapseButton;
    QSpacerItem *horizontalSpacer;

private slots:
    void emitSigRefresh();
    void emitSigExpand();
    void emitSigCollapse();
};

#endif // QJDAREAHEADWIDGET_H

#ifndef QJDFUNCATIONHEADWIDGET_H
#define QJDFUNCATIONHEADWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QToolButton>
#include <QSpacerItem>
class QJDFuncationHeadWidget : public QFrame
{
    Q_OBJECT
public:
    explicit QJDFuncationHeadWidget(QFrame *parent = 0);
    
signals:
    void sigExpandClicked();
    void sigCollapseClicked();
public slots:
private:
    QLabel *textLabel;
    QToolButton *expandButton;
    QToolButton *collapseButton;
    QSpacerItem *horizontalSpacer;
private slots:
    void emitSigExpand();
    void emitSigCollapse();
};

#endif // QJDFUNCATIONHEADWIDGET_H

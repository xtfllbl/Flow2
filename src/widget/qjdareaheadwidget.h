#ifndef QJDAREAHEADWIDGET_H
#define QJDAREAHEADWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QToolButton>
#include <QSpacerItem>

class QJDAreaHeadWidget : public QFrame
{
    Q_OBJECT
public:
    explicit QJDAreaHeadWidget(QFrame *parent = 0);
    
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

#endif // QJDAREAHEADWIDGET_H

#ifndef QJDMDISUBLISTWIDGET_H
#define QJDMDISUBLISTWIDGET_H

#include <QListWidget>
#include <QMenu>
#include <QListWidgetItem>

class QJDMdiSubListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QJDMdiSubListWidget(QWidget *parent = 0);
    
signals:
    void sigMidButtonClicked(QListWidgetItem *item);
    void sigDelFlowClicked(QList<QListWidgetItem *>);
    void sigOpenCloseFlowClicked(QList<QListWidgetItem *>);
public slots:
private:
    QMenu *menu;
    QAction *actDelFlow;
    QAction *actOpenCloseFlow;
    QListWidgetItem *rightClickedItem;

    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void midClickedSlot(QListWidgetItem *item);
    void delFlowSlot();
    void openCloseFlowSlot();
};

#endif // QJDMDISUBLISTWIDGET_H

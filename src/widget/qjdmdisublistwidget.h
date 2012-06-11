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
    void sigMdiSubClose();

public slots:
private:
    QMenu *menu;
    QAction *actDelFlow;
    QAction *actOpenCloseFlow;
    QListWidgetItem *rightClickedItem;

    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *);
    void dragEnterEvent (QDragEnterEvent *event);
    void dropEvent (QDropEvent *event);
private slots:
    void midClickedSlot(QListWidgetItem *item);
    void delFlowSlot();
    void openCloseFlowSlot();
};

#endif // QJDMDISUBLISTWIDGET_H

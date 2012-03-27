#ifndef QJDMDISUBLISTWIDGET_H
#define QJDMDISUBLISTWIDGET_H

#include <QListWidget>

class QJDMdiSubListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QJDMdiSubListWidget(QWidget *parent = 0);
    
signals:
    void sigMidButtonClicked(QListWidgetItem *item);
public slots:
private:
    void mousePressEvent(QMouseEvent *event);
private slots:
    void itemClickedSlot(QListWidgetItem *item);
};

#endif // QJDMDISUBLISTWIDGET_H

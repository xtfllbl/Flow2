#ifndef QJDPROCESSWIDGET_H
#define QJDPROCESSWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QTableWidget>

class QJDProcessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QJDProcessWidget(QWidget *parent = 0);
    
signals:
private:
    QMenuBar *menuBar;
    QTableWidget *tableWidget;

public slots:
    void addTask(QString pid);
};

#endif // QJDPROCESSWIDGET_H

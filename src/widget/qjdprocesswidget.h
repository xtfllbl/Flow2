#ifndef QJDPROCESSWIDGET_H
#define QJDPROCESSWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QTableWidget>
#include <QProcess>
#include <QHash>
#include "qjdtextbrowser.h"

class QJDProcessTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit QJDProcessTableWidget(QWidget *parent = 0);

signals:
private:
    void contextMenuEvent(QContextMenuEvent *);
    QMenu *menu;
    QAction *actShowLog;
    QAction *actDelLog;
//    QAction *actContinue;
//    QAction *actStop;
//    QAction *actPause;
//    QAction *actKill;
    QAction *actTer;
    QJDTextBrowser *textBrowser;

public slots:
    void saveHistory();
    void loadHistory();

    void clearHistorySlot();
private slots:
    void showLogSlot();
    void delSlot();

//    void killProcess();
    void terProcess();
//    void hanProcess();
//    void stopProcess();
//    void conProcess();
    void send_to_selected(int);
    void sendsig(int, int);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QJDProcessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QJDProcessWidget(QWidget *parent = 0);
    
signals:
private:
    QMenuBar *menuBar;
    QJDProcessTableWidget *tableWidget;

    QHash<int,int> hashRow;

public slots:
    void addTask(QString pid,QString name,QString log);
    void changeStatus(int pid, int exitCode, QProcess::ExitStatus exitStatus);
private slots:

};





#endif // QJDPROCESSWIDGET_H

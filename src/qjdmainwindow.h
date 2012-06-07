#ifndef QJDMAINWINDOW_H
#define QJDMAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "widget/QJD/qjdlabel.h"
#include "widget/qjdpropertywidget.h"
#include "widget/qjdareawidget.h"
#include "widget/qjdfuncationwidget.h"
#include "widget/qjdtabwidget.h"
#include "widget/qjdmdi.h"
#include "widget/qjdareaheadwidget.h"
#include "widget/qjdfuncationheadwidget.h"

namespace Ui {
class QJDMainWindow;
}

class QJDMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit QJDMainWindow(QWidget *parent = 0);
    ~QJDMainWindow();
    
    bool deleteDir(const QString dirName);

private slots:
    void on_actionExit_triggered();
    void setWidgetVisible(int);
    void on_actionNewArea_triggered();
    void on_actionNewLine_triggered();
    void on_actionNewFlow_triggered();
    void on_actionSetDataHome_triggered();

    void showExistFlow();

    void on_actionExcuteFlow_triggered();

    void on_actionDelArea_triggered();

    void on_actionDelLine_triggered();

    void on_actionDelFlow_triggered();

    void refreshList();

    void openFlowSlot();
    void excuteFlowSlot();

    void on_actionJob_Viewer_triggered();

private:
    Ui::QJDMainWindow *ui;
    QSettings settings;

    QString _HOME_DIR;
//    QHash<QString ,QString> stringPath;
    QJDAreaWidget *areaWidget;
    QJDAreaHeadWidget *areaHeadWidget;
    QWidget *areaSplitterWidget;

    QJDTabWidget *tabWidget;
    QJDPropertyWidget *propertyWidget;
    QJDMdi *mdiWidget;
    QJDFuncationWidget *funcationWidget;
    QJDFuncationHeadWidget *funcationHeadWidget;
    QWidget *funcationSplitterWidget;
    QJDLabel *pathLabel1;
    QJDLabel *pathLabel2;
    QSplitter *splitter;



    void setHomeDir(QString);
    QString getHomeDir();
    void setAreaWidget(QString areaString,QString areaPath,QStringList lineStringList,QStringList linePathList,
                       QVector<QStringList> flowStringList,QVector<QStringList> flowPathList);

public slots:
    void enableNew(int level);
    void creatNewArea(QString areaName);
    void creatNewLine(QString lineName);
    void creatNewFlow(QString flowName);
};

#endif // QJDMAINWINDOW_H

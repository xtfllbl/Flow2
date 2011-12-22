#ifndef QJDMAINWINDOW_H
#define QJDMAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "widget/qjdlabel.h"
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
    
private slots:
    void on_actionExit_triggered();
    void setWidgetVisible(int);

private:
    Ui::QJDMainWindow *ui;

    QHash<QString ,QString> stringPath;
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


    void setDir();
    void setAreaWidget(QString areaString,QString areaPath,QStringList lineStringList,QStringList linePath,
                       QStringList flowStringList,QStringList flowPath);
};

#endif // QJDMAINWINDOW_H

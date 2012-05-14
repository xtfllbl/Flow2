#ifndef QJDMDI_H
#define QJDMDI_H

#include <QMdiArea>
#include <QMdiSubWindow>
#include "qjdmdisublistwidget.h"
#include <QtXml>

class QJDMdi : public QMdiArea
{
    Q_OBJECT
public:
    QJDMdi();
private:
    QHash<QMdiSubWindow *,QJDMdiSubListWidget *> hashWindow_List;
//    QList<QStringList> moduleList; //whats this monster,毕竟一边处理一边写文件不太好
    QString moduleName;
    QString propertyNameAndValue;
    QStringList propertyNameAndValueList;

    QStringList moduleNameList;
    QList<QStringList> module_PropertyList;

    QString listWidgetItemName;
    QString listWidgetItemTooltip;

    QString FLOW_PATH;
    QStringList ITEM_PATH_LIST;
    QStringList ITEM_STATUS_LIST;

public slots:
    void newSubWindow(QString subName,QString linePath);
    void showExistSubWindow(QString flowName,QString flowPath);
    void addFlow(QString name,QString xmlPath);
    void excuteFlow();

    /// 中键单击,弹出编辑的窗口 QJDFunArguWidget
    /// 窗口中要有保存按钮,按下之后改写文件
    /// 文件路径就为listwidget的toottip
    void showFunArgu(QString name,QString path);  // 需要名称和路径
private slots:
    void windowChoosed(QMdiSubWindow*);
    bool processFlowXML(QStringList);
    QString checkFileIfExist(QString fileName);

    void parseModuleElement(QDomElement const&);
    void parsePropertyElement(const QDomElement &element);

    QString outputMainFlowArguFile(QStringList,QList<QStringList>);
    void runProcess(QString arg);
    void listWidgetItemMidClicked(QListWidgetItem *);
    bool delFlow(QList<QListWidgetItem *>);
    void openCloseFlow(QList<QListWidgetItem *>);

    bool refreshPosFile();
};

#endif // QJDMDI_H

#ifndef QJDTREEVIEW_H
#define QJDTREEVIEW_H

#include <QTreeWidget>

// It is impossible to put the modle here too
class QJDAreaWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QJDAreaWidget();

//    bool isArea;
//    bool isLine;
//    bool isFlow;

    void setLevel(int level);
    int level();

private:
    int _level;
    QString absolutePath;
    QString homeDirPath;

    QMenu *menu;
    QAction *actNewArea;
    QAction *actNewLine;
    QAction *actNewFlow;
signals:
    void sigItemPath(QString path);
    void sigLevel(int);

    void sigActNewAreaClicked();
    void sigActNewLineClicked();
    void sigActNewFlowClicked();
    void sigExistFlow();
private slots:
    void setContextMenu(int);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *);

    void actNewAreaClicked();
    void actNewLineClicked();
    void actNewFlowClicked();
public slots:
    void returnPath(QTreeWidgetItem*,int);
    QString getAbsolutePath();
    void setHome(QString path);
    void showContextMenu(QPoint );

};

#endif // QJDTREEVIEW_H

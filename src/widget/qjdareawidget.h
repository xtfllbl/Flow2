#ifndef QJDTREEVIEW_H
#define QJDTREEVIEW_H

#include <QTreeWidget>
#include <QPlastiqueStyle>

// It is impossible to put the modle here too
class QJDAreaWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QJDAreaWidget();

//    bool isArea;
//    bool isLine;
//    bool isFlow;
    QString returnPathStr;

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
    QAction *actDelArea;
    QAction *actDelLine;
    QAction *actDelFlow;

    QAction *actOpenFlow;
    QAction *actExcuteFlow;
signals:
    void sigItemPath(QString path);
    void sigLevel(int);

    void sigActNewAreaClicked();
    void sigActNewLineClicked();
    void sigActNewFlowClicked();
    void sigActDelAreaClicked();
    void sigActDelLineClicked();
    void sigActDelFlowClicked();

    void sigActOpenFlowClicked();
    void sigActExcuteFlowClicked();

    void sigExistFlow();
private slots:
    void setContextMenu(int);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *);

    void actNewAreaClicked();
    void actNewLineClicked();
    void actNewFlowClicked();
    void actDelAreaClicked();
    void actDelLineClicked();
    void actDelFlowClicked();

    void actOpenFlowClicked();
    void actExcuteFlowClicked();
public slots:
    void returnPath(QTreeWidgetItem*,int);
    QString getAbsolutePath();
    void setHome(QString path);

    void removeCurrentArea();
    void removeCurrentLine_Flow();

};

class QJDMenuStyle: public QPlastiqueStyle
{
    Q_OBJECT
  public:
    int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const {
        int s = QPlastiqueStyle::pixelMetric(metric, option, widget);
        if (metric == QStyle::PM_SmallIconSize) {
            s = 24;
        }
        return s;
    }
};

#endif // QJDTREEVIEW_H

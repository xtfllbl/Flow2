#ifndef QJDPROPERTYWIDGET_H
#define QJDPROPERTYWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QMenu>
#include <QFileInfo>

class QJDPropertyWidget : public QTableView
{
    Q_OBJECT
public:
    QJDPropertyWidget();
    
signals:
    
public slots:
    void setPropertyData(QString path);
private:
    QString dirSize(const QString &sDirPath);
    QString fileSize(qint64 size);

    QMenu *menu;
    QAction *actDelete;
    QString currentPath;
    QString basePath;
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void delFile();
    void deleteDirectory(QFileInfo fileList);

};

#endif // QJDPROPERTYWIDGET_H

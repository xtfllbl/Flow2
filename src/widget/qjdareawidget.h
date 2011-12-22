#ifndef QJDTREEVIEW_H
#define QJDTREEVIEW_H

#include <QTreeWidget>

// It is impossible to put the modle here too
class QJDAreaWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QJDAreaWidget();
private:

signals:
    void sigItemPath(QString path);

public slots:
    void itemPath(QTreeWidgetItem*);
};

#endif // QJDTREEVIEW_H

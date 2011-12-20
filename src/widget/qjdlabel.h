#ifndef QJDLABEL_H
#define QJDLABEL_H

#include <QLabel>
#include <QTreeWidgetItem>

class QJDLabel : public QLabel
{
    Q_OBJECT
public:
    QJDLabel();

public slots:
    void setTreeItemText(QTreeWidgetItem*,int);
};

#endif // QJDLABEL_H

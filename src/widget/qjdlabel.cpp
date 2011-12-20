#include "qjdlabel.h"
#include <QDebug>
QJDLabel::QJDLabel()
{
//    qDebug()<<"jdlabel";
}

void QJDLabel::setTreeItemText(QTreeWidgetItem *item, int index)
{
    qDebug()<<"jdlabel::setTreeitemtext"<<index;
    QString text=item->text(0);
    this->setText(text);
}

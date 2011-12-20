#ifndef QJDPROPERTYWIDGET_H
#define QJDPROPERTYWIDGET_H

#include <QWidget>
#include <QTableView>

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


    
};

#endif // QJDPROPERTYWIDGET_H

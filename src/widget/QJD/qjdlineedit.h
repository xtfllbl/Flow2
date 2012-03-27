#ifndef QJDLINEEDIT_H
#define QJDLINEEDIT_H

#include <QLineEdit>

class QJDLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QJDLineEdit(QWidget *parent = 0);

    int propertyInt;
    void setPropertyList(QString,QString,QString,QString);
signals:
    void sigLineEditChanged(int, QStringList);
public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
private slots:
    void emitEditFinished();
};

#endif // QJDLINEEDIT_H

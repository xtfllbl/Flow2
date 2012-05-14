#ifndef QJDSPINBOX_H
#define QJDSPINBOX_H

#include <QSpinBox>

class QJDSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit QJDSpinBox(QWidget *parent = 0);
    int propertyInt;
    void setPropertyList(QStringList);
signals:
    void sigValueChanged(int, QStringList);

public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;
private slots:
    void emitValueChanged(QString);
};

#endif // QJDSPINBOX_H

#ifndef QJDCOMBOBOX_H
#define QJDCOMBOBOX_H

#include <QComboBox>

class QJDComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QJDComboBox(QWidget *parent = 0);
    int propertyInt;
    void setPropertyList(QStringList);
signals:
    void sigComboBoxChanged(int, QStringList);

public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;
private slots:
    void emitIndexChanged(QString);
};

#endif // QJDCOMBOBOX_H

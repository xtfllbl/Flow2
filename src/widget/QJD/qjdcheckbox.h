#ifndef QJDCHECKBOX_H
#define QJDCHECKBOX_H

#include <QCheckBox>

class QJDCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit QJDCheckBox(QWidget *parent = 0);
    int propertyInt;
    void setPropertyList(QStringList);
signals:
    void sigCheckBoxChanged(int, QStringList);

public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;
private slots:
    void emitEditFinished(int);

};

#endif // QJDCHECKBOX_H

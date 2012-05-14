#ifndef QJDBUTTONGROUP_H
#define QJDBUTTONGROUP_H

#include <QButtonGroup>
#include <QAbstractButton>

class QJDButtonGroup : public QButtonGroup
{
    Q_OBJECT
public:
    explicit QJDButtonGroup(QObject *parent = 0);
    int propertyInt;
    void setPropertyList(QStringList);
signals:
    void sigButtonGroupChecked(int, QStringList);

public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;
private slots:
    void emitEditFinished(QAbstractButton*);
};

#endif // QJDBUTTONGROUP_H

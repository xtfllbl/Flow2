#ifndef QJDRADIOBUTTON_H
#define QJDRADIOBUTTON_H

#include <QRadioButton>

/// 与其他控件不一样, 麻烦就麻烦在多个来整合一个
class QJDRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit QJDRadioButton(QWidget *parent = 0);
    int propertyInt;
    void setPropertyList(QStringList);
signals:
    void sigRadioChanged(int, QStringList);

public slots:
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;
private slots:
    void emitStateChanged(bool);
};

#endif // QJDRADIOBUTTON_H

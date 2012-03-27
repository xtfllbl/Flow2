#ifndef QJDFILESAVELINE_H
#define QJDFILESAVELINE_H

#include <QObject>
#include <QLineEdit>
#include <QToolButton>

class QJDFileSaveLine : public QWidget
{
    Q_OBJECT
public:
    explicit QJDFileSaveLine(QWidget *parent = 0);
    void setPropertyList(QString propertyName, QString desc, QString dataType, QString displayType);
    int propertyInt;

signals:
    void sigLineEditChanged(int, QStringList);

public slots:

    void fileButtonPressed();
    void setText(QString text);
    QString text();

private slots:
    void emitEditFinished();
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;

    QLineEdit *lineEdit;
    QToolButton *fileButton;

    QString fileName;
};

#endif // QJDFILESAVELINE_H

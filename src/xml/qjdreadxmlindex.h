#ifndef READXMLINDEX_H
#define READXMLINDEX_H

#include <QObject>
#include <QIODevice>
#include <QtXml>

class QJDReadXMLIndex : public QObject
{
    Q_OBJECT
public:
    explicit QJDReadXMLIndex(QObject *parent = 0);

signals:
    void sigModuleData(QString modulename,QString moduleversion,QString moduleXMLpath);

public slots:
    bool read(QIODevice *device);
    void parsePropertyElement(const QDomElement &element);

    void setModule(QString moduleName,QString moduleGroup,QString modulePath,
                       QString moduleDesc, QString moduleType);
    QStringList getModuleName();
    QStringList getModuleGroup();
    QStringList getModulePath();
    QStringList getModuleDesc();
    QStringList getModuleType();
private:
    QDomDocument domDocument;

    QString name;
    QString group;
    QString path;
    QString desc;
    QString type;

    QStringList allModuleName;
    QStringList allModuleGroup;
    QStringList allModulePath;
    QStringList allModuleDesc;
    QStringList allModuleType;
    void nodeClear();
};

#endif // READXMLINDEX_H

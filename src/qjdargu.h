#ifndef QJDARGU_H
#define QJDARGU_H

#include <QObject>
#include <QApplication>

class QJDArgu : public QObject
{
    Q_OBJECT
public:
    explicit QJDArgu(QObject *parent = 0);

    QString JD_FlowDataDir;
    QString JD_FlowDir;
    QString JD_FileType;
};

#endif // QJDARGU_H

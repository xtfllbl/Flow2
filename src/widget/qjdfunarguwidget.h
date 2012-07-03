#ifndef QJDFUNARGUWIDGET_H
#define QJDFUNARGUWIDGET_H

#include <QWidget>
#include <QtXml>
#include <QtGui>

class QJDFunArguWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QJDFunArguWidget(QWidget *parent = 0);
    
signals:
    void sigHighlightSave();
public slots:
    bool setData(QString name,QString path);
    void saveArgToXml();
private:

    QDomDocument domDocument;

    QString modulename;
    QString moduleversion;
    QString property;
    QString desc;
    QString datatype;
    QString displaytype;
    QString displayvalue;
    QString optionstring;

    QString ARGU_FILE_PATH;
    QString DATA_PATH;

    QVBoxLayout *finalLayout;
    QDomDocument doc;


//    QHash<QWidget *,QString> hashWhatIsThisWidget;
//    QList<QWidget *> widgetList;
    QList<QStringList> propertyList;

    bool readFile(QIODevice *device);
    bool prepareWriteFile(QIODevice *device);
    void parsePropertyElement(const QDomElement &element);
    void creatWidgetItem(QString property,QString desc,QString datatype,QString displaytype,QString displayvalue
                         ,QString optionstring);
    void finishCreatUI();
    bool write();

private slots:
    void changeDisplayValue(int propertyListInt ,QStringList changeList);
};

#endif // QJDFUNARGUWIDGET_H

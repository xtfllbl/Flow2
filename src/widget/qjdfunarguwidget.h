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
    
public slots:
    void setData(QString name,QString path);
private:

    QDomDocument domDocument;

    QString modulename;
    QString property;
    QString desc;
    QString datatype;
    QString displaytype;
    QString displayvalue;
//    QStringList optiontext;
//    QStringList optionvalue;

    QVBoxLayout *finalLayout;


//    QHash<QWidget *,QString> hashWhatIsThisWidget;
//    QList<QWidget *> widgetList;
    QList<QStringList> propertyList;

    bool readFile(QIODevice *device);
    void parsePropertyElement(const QDomElement &element);
    void creatWidgetItem(QString property,QString desc,QString datatype,QString displaytype,QString displayvalue);
    void finishCreatUI();
private slots:
    void changeDisplayValue(int propertyListInt ,QStringList changeList);
    void saveArgToXml();
};

#endif // QJDFUNARGUWIDGET_H

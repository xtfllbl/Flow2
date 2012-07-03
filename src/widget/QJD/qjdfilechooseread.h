#include <QObject>
#include <QLineEdit>
#include <QToolButton>
#include <QListWidget>
#include <QLabel>

class QJDFileChooseRead : public QWidget
{
    Q_OBJECT
public:
    explicit QJDFileChooseRead(QWidget *parent = 0);
    void setPropertyList(QStringList);
    int propertyInt;

signals:
    void sigFileChooseReadEditChanged(int, QStringList);
public slots:
    void setTypeData(QString dataDirPath);

private slots:
    void setFileList(QListWidgetItem*);
    void emitEditFinished(QListWidgetItem*);

private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;

    QString DATA_PATH;
    QString DIR_PATH;

    QLabel *typeLabel;
    QLabel *listLabel;

    QListWidget *leftListWidget;
    QListWidget *rightListWidget;


};

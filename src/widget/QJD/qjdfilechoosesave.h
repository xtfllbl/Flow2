#include <QObject>
#include <QLineEdit>
#include <QToolButton>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>

class QJDFileChooseSave : public QWidget
{
    Q_OBJECT
public:
    explicit QJDFileChooseSave(QWidget *parent = 0);
    void setPropertyList(QStringList);
    int propertyInt;

signals:
    void sigFileChooseSaveEditChanged(int, QStringList);
public slots:

    void setComboBoxCurrentIndex(int);
    void addComboBoxItem(QString);
    void setLineText(QString);
    void setDataPath(QString);
private slots:
    void emitEditFinished();
private:
    QString PROPERTY;
    QString DESC;
    QString DATATYPE;
    QString DISPLAYTYPE;
    QString OPTION;

    QString DATA_PATH;
    QString DIR_PATH;

    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QLabel *upLabel;
    QLabel *downLabel;


};

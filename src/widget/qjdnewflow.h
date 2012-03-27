#ifndef QJDNEWFLOW_H
#define QJDNEWFLOW_H
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
class QJDNewFlow: public QDialog
{
    Q_OBJECT

public:
    explicit QJDNewFlow(QWidget *parent = 0);
    QString flowName;
private:
    QPushButton *applyBtn;
    QPushButton *cancleBtn;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *spacerLeft;
    QSpacerItem *spacerRight;
signals:
    void sigFlowName(QString);
public slots:
    void apply();
    void cancle();
    void setFlowName();
    QString getFlowName();
};

#endif // QJDNEWFLOW_H

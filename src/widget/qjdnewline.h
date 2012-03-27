#ifndef QJDNEWLINE_H
#define QJDNEWLINE_H
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
class QJDNewLine: public QDialog
{
    Q_OBJECT

public:
    explicit QJDNewLine(QWidget *parent = 0);
    QString lineName;
private:
    QPushButton *applyBtn;
    QPushButton *cancleBtn;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *spacerLeft;
    QSpacerItem *spacerRight;
signals:
    void sigLineName(QString);
public slots:
    void apply();
    void cancle();
    void setLineName();
    QString getLineName();
};

#endif // QJDNEWLINE_H

#ifndef QJDNEWAREA_H
#define QJDNEWAREA_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

class QJDNewArea : public QDialog
{
    Q_OBJECT
public:
    explicit QJDNewArea(QWidget *parent = 0);

    QString areaName;
private:
    QPushButton *applyBtn;
    QPushButton *cancleBtn;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *spacerLeft;
    QSpacerItem *spacerRight;
signals:
    void sigAreaName(QString);
public slots:
    void apply();
    void cancle();
    void setAreaName();
    QString getAreaName();
};

#endif // QJDNEWAREA_H

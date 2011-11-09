#include "qjdmainwindow.h"
#include "ui_qjdmainwindow.h"

QJDMainWindow::QJDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QJDMainWindow)
{
    ui->setupUi(this);
}

QJDMainWindow::~QJDMainWindow()
{
    delete ui;
}

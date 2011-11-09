#ifndef QJDMAINWINDOW_H
#define QJDMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class QJDMainWindow;
}

class QJDMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit QJDMainWindow(QWidget *parent = 0);
    ~QJDMainWindow();
    
private:
    Ui::QJDMainWindow *ui;
};

#endif // QJDMAINWINDOW_H

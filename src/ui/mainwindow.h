#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QTabWidget *mpTabWidget;
    Ui::MainWindow *ui;
    /// ----------------------------------------------------
    /// 添加页
    void AddTab();
};

#endif // MAINWINDOW_H

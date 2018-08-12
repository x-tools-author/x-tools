#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

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
    /// 创建菜单
    void InitMenu();

};

#endif // MAINWINDOW_H

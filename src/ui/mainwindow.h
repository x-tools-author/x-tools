/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLabel>

class SAKVersion;
class UpdateManager;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QTabWidget* mpTabWidget     = nullptr;
    Ui::MainWindow* ui          = nullptr;
    QMenu* toolsMenu            = nullptr;
    SAKVersion* versionDialog   = nullptr;
    UpdateManager* updateManager= nullptr;
    /// ----------------------------------------------------
    void AddTab();
    void InitMenu();
    void AddTool();

    void addTool(QString toolName, QWidget *toolWidget);
private slots:
    void AboutQt();
    void About();
    void styleActionTriggered();
};

#endif // MAINWINDOW_H

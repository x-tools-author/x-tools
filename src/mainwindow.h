/***************************************************************************************************
 * Copyright 2018-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMetaEnum>
#include <QSettings>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

#ifdef Q_OS_WIN
protected:
    void closeEvent(QCloseEvent* event) override;
#endif

private:
    struct SettingsKeyContext
    {
        const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
    } mSettingsKey;
    struct NavContext
    {
        QButtonGroup* bg;
        QIcon icon;
        QString name;
        QWidget* page;
        QToolBar* tb;
    };

private:
    const QLoggingCategory mLoggingCategory{"sak.mainwindow"};
    Ui::MainWindow* ui;

private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initOptionMenuAppStyleMenu(QMenu* optionMenu);
    void initOptionMenuMainWindowMenu(QMenu* optionMenu);
    void initOptionMenuSettingsMenu(QMenu* optionMenu);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    void initOptionMenuHdpiPolicy(QMenu* optionMenu);
#endif
    void initOptionMenuPalette(QMenu* optionMenu);
    void initWindowMenu();
    void initLanguageMenu();
    void initHelpMenu();
    void initLinksMenu();
    void initDemoMenu();
    void initNav();
    void initNav(const NavContext& ctx);
    void initStatusBar();

    void aboutSoftware();
    void clearConfiguration();
    void rebootRequestion();
    void showHistory();
    void showQrCode();
    void showDonation();
    void createQtConf();

private slots:
    void onImportActionTriggered();
    void onExportActionTriggered();
};

#endif // MAINWINDOW_H

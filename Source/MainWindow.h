/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <QMetaEnum>
#include <QSettings>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTranslator>

#include "xToolsMainWindow.h"

class MainWindow : public xToolsMainWindow
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

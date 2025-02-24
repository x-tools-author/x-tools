/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QActionGroup>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QStyleFactory>

class IOPage;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

    static QIcon cookedIcon(const QString& svgFileName);
    static QIcon cookedIcon(const QIcon& icon);
    void updateWindowTitle();

protected:
    QMenu* m_fileMenu;
    QMenu* m_optionMenu;
    QMenu* m_languageMenu;
    QMenu* m_helpMenu;
    QMenu* m_appStyleMenu;
    QMenu* m_colorSchemeMenu;

    QAction* m_themeAction{nullptr};
    QAction* m_exitAction;

    QAction* m_gitHubAction;
    QAction* m_giteeAction;
    QAction* m_qqGroupAction;
    QAction* m_aboutAction;
    QAction* m_aboutQtAction;

protected:
    void closeEvent(QCloseEvent* event) override;
    static QString qtConfFileName();

private:
    QActionGroup* m_appStyleActionGroup;
    QActionGroup* m_languageActionGroup;
    QActionGroup* m_appPaletteActionGroup;

    enum class WindowGrid { Grid1x1, Grid1x2, Grid2x1, Grid2x2 };
    struct SettingsKeys
    {
        const QString windowGrid{"MainWindow/windowGrid"};
        const QString isTextBesideIcon{"MainWindow/isTextBesideIcon"};
        const QString pageIndex{"MainWindow/pageIndex"};
        const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
        const QString useSystemProxy{"MainWindow/useSystemProxy"};
    } m_settingsKey;

    QMenu* m_toolMenu;
    WindowGrid m_windowGrid{WindowGrid::Grid1x1};
    IOPage* m_ioPage00;
    IOPage* m_ioPage01;
    IOPage* m_ioPage10;
    IOPage* m_ioPage11;

private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initViewMenu();
    void initHelpMenu();
    void initLinksMenu();

    void updateGrid(WindowGrid grid);
    static void showHistory();
    static void showQrCode();

    void load(const QString& fileName = QString()) const;
    void save(const QString& fileName = QString()) const;
    void onSaveActionTriggered() const;
    void onImportActionTriggered();
    void onExportActionTriggered();

    void initMenuFile();
    void initMenuOption();
    void initMenuLanguage();
    void initMenuHelp();

    void initOptionMenuAppStyleMenu();
    void initOptionMenuSettingsMenu();
    void initOptionMenuHdpiPolicy();
    void initOptionMenuColorScheme();

    void onHdpiPolicyActionTriggered(int policy);
    void onAboutActionTriggered();

    bool tryToReboot();
    void createQtConf();
    void showQqQrCode();
};

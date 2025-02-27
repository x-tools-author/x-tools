/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMainWindow>
#include <QMenu>

class IOPage;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    enum class WindowGrid { Grid1x1, Grid1x2, Grid2x1, Grid2x2 };
    struct SettingsKeys
    {
        const QString windowGrid{"MainWindow/windowGrid"};
        const QString pageIndex{"MainWindow/pageIndex"};
        const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
        const QString useSystemProxy{"MainWindow/useSystemProxy"};
    } m_settingsKey;

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
    void initMenuLanguage();
    void initViewMenu();
    void initLinksMenu();
    void initHelpMenu();

    void initOptionMenuAppStyleMenu(QMenu* optionMenu);
    void initOptionMenuSettingsMenu(QMenu* optionMenu);
    void initOptionMenuHdpiPolicy(QMenu* optionMenu);
    void initOptionMenuColorScheme(QMenu* optionMenu);

    void updateGrid(WindowGrid grid);
    void showHistory();
    void showQrCode();
    void tryToReboot();
    QString qtConfFileName();
    void createQtConf();

    void load(const QString& fileName = QString()) const;
    void save(const QString& fileName = QString()) const;

    void onSaveActionTriggered() const;
    void onImportActionTriggered();
    void onExportActionTriggered();
    void onAboutActionTriggered();
};

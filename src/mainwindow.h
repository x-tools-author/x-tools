/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QActionGroup>
#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QUrl>

class Page4x4;
class QStackedWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

    void load(const QString& fileName = QString()) const;
    void save(const QString& fileName = QString()) const;
    void updateGrid(int grid);
    void moveToCenter();

protected:
    void closeEvent(QCloseEvent* event) override;
    virtual QUrl storeUrl() const;

protected:
    Page4x4* m_page4x4{nullptr};
    QStackedWidget* m_stackedWidget{nullptr};
    QToolBar* m_toolBar{nullptr};
    QActionGroup* m_toolBarActionGroup{nullptr};

private:
    struct SettingsKeys
    {
        const QString pageIndex{"MainWindow/pageIndex"};
        const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
        const QString useSystemProxy{"MainWindow/useSystemProxy"};
        const QString staysOnTop{"MainWindow/staysOnTop"};
        const QString language{"MainWindow/pageIndex"};
    } m_settingsKey;

private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initOptionMenuAppStyleMenu(QMenu* optionMenu);
    void initOptionMenuSettingsMenu(QMenu* optionMenu);
    void initOptionMenuHdpiPolicy(QMenu* optionMenu);
    void initOptionMenuColorScheme(QMenu* optionMenu);
    void initMenuLanguage();
    void initViewMenu();
    void initViewMenuStayOnTop(QMenu* viewMenu);
    void initHelpMenu();

    void showHistory();
    void showQrCode();
    void tryToReboot();
    void setupPages();
    void addPage(const QString& name,
                 QWidget* page,
                 const QString& iconName,
                 const QString& tooltip = QString());

    // HDPI Policy for Windows
    QString qtConfFileName();
    void createQtConf();

    void onSaveActionTriggered() const;
    void onImportActionTriggered();
    void onExportActionTriggered();
    void onAboutActionTriggered();
};

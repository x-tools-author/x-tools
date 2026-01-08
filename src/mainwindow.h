/***************************************************************************************************
 * Copyright 2018-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QUrl>

#include "common/xui.h"

class Page;
class LayoutManager;
class MainWindow : public xUi
{
    Q_OBJECT
public:
    enum class WindowGrid { Grid1x1, Grid1x2, Grid2x1, Grid2x2 };

public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

    void load(const QString& fileName = QString());
    void save(const QString& fileName = QString()) const;
    void updateGrid(WindowGrid grid);
    void moveToCenter();
    void hideHistoryAction();
    void showLiteMode();

    // Just for xFlow
    Q_INVOKABLE void outputBytes(const QString& txt, int channel);
    Q_INVOKABLE void clearOutput(int channel);

protected:
    void closeEvent(QCloseEvent* event) override;
    virtual QUrl storeUrl() const;

protected:
    Page* m_ioPage00;
    Page* m_ioPage01;
    Page* m_ioPage10;
    Page* m_ioPage11;
    QAction* m_historyAction{nullptr};
    LayoutManager* m_layoutManager{nullptr};
    QMenu* m_viewMenu{nullptr};

private:
    struct SettingsKeys
    {
        const QString windowGrid{"MainWindow/windowGrid"};
        const QString pageIndex{"MainWindow/pageIndex"};
        const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
        const QString useSystemProxy{"MainWindow/useSystemProxy"};
        const QString staysOnTop{"MainWindow/staysOnTop"};
    } m_settingsKey;

    WindowGrid m_windowGrid{WindowGrid::Grid1x1};

private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initViewMenu();
    void initViewMenuGrid(QMenu* viewMenu);
    void initHelpMenu();

    void showHistory();
    void showQrCode();
    void visitOnlineDocumentation();

    // HDPI Policy for Windows
    QString qtConfFileName();
    void createQtConf();

    void onSaveActionTriggered() const;
    void onImportActionTriggered();
    void onExportActionTriggered();
    void onAboutActionTriggered();
};

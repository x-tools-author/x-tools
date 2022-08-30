/******************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QSettings>
#include <QMetaEnum>
#include <QTabWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTranslator>
#include <QSqlDatabase>

namespace Ui {
    class SAKMainWindow;
}

class SAKUpdateManager;
class SAKMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SAKMainWindow(QSettings *settings,
                           QSqlDatabase *sqlDatabase,
                           QWidget *parent = Q_NULLPTR);
    ~SAKMainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private:
    struct SettingsKeyContext {
        QString enableTestPage;
        QString currentTabPage;
        QString exitToSystemTray;
    }mSettingsKeyContext;

    QMenu *mWindowsMenu;
    QAction *mTestPageAction;
    SAKUpdateManager *mUpdateManager;
    const QString mSettingKeyEnableTestPage;
    QSettings *mSettings;
    QSqlDatabase *mSqlDatabase;
    QActionGroup *mActionGroup;
    QActionGroup *mLanguagesActionGroup;
private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initOptionMenuAppStyleMenu(QMenu *optionMenu);
    void initOptionMenuMainWindowMenu(QMenu *optionMenu);
    void initOptionMenuSettingsMenu(QMenu *optionMenu);
    void initOptionMenuTestPageAction(QMenu *optionMenu);
    void initWindowMenu();
    void initLanguageMenu();
    void initHelpMenu();
    void initLinksMenu();
    void initDemoMenu();
#ifdef SAK_IMPORT_MODULE_USER
    void initUserMenu();
#endif

    void aboutQsak();
    void removeRemovableDebugPage(int index);
    void appendWindowAction(QWidget *page);
    void testPageActionTriggered();
    void clearConfiguration();
    void rebootRequestion();
    void showReleaseHistoryActionDialog();
    QString tabPageName(int type);
    QWidget *debugPage(QObject *sender);
    void showQrCodeDialog();
    void onDonationActionTriggered();
private slots:
    /**
     * @brief activePage: Active the debugging page,
     * the interface must be called by signal,
     * which is emited by actions of windows menu.
     */
    void activePage();

    /**
     * @brief installLanguage: Change the language packet of application,
     * the interface must be called by signal,
     * which is emited by actions of language menu.
     */
    void installLanguage();

    /**
     * @brief openDebugPageWidget: Show a deugging window,
     * the interface must be called by singal,
     * which is emited by actions of new debugging window menu.
     */
    void openDebugPageWidget();

    /**
     * @brief appendRemovablePage: append a page to tab widget, t
     * he interface must be called by signal,
     * which is emited by actions of new debugging page menu.
     */
    void appendRemovablePage();
private:
    Ui::SAKMainWindow *mUi;
    QTabWidget *mTabWidget;
};

extern SAKMainWindow *sakMainWindow;

#endif // MAINWINDOW_H

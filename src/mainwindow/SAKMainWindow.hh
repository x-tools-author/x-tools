/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QMetaEnum>
#include <QTabWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class SAKMainWindow;
}

class SAKMainWindowQrCodeView;
class SAKUpdateManager;
class SAKMainWindowMoreInformationDialog;
class SAKMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SAKMainWindow(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindow();

    static const QString settingKeyClearConfiguration();
private:
    QMenu *mToolsMenu;
    QMenu *mWindowsMenu;
    QAction *mTestPageAction;
    QAction *mDefaultStyleSheetAction;
    SAKUpdateManager *mUpdateManager;
    SAKMainWindowMoreInformationDialog *mMoreInformation;
    SAKMainWindowQrCodeView *mQrCodeDialog;
    const QString mSettingKeyEnableTestPage;
    const QString mSettingKeyClearConfiguration;
private: 
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initWindowMenu();
    void initLanguageMenu();
    void initHelpMenu();
    void initLinksMenu();

    void changeStylesheet(QString styleSheetName);
    void changeAppStyle(QString appStyle);

    void about();
    void removeRemovableDebugPage(int index);
    QWidget *debugPageFromType(int type);
    void appendWindowAction(QWidget *page);
    void testPageActionTriggered();
    void clearConfiguration();
    void rebootRequestion();
private slots:
    /**
     * @brief showToolWidget: Show a tool widget, the interface must be called by signal,
     * which is emited by actions of tools menu.
     */
    void showToolWidget();

    /**
     * @brief activePage: Active the debugging page, the interface must be called by signal,
     * which is emited by actions of windows menu.
     */
    void activePage();

    /**
     * @brief installLanguage: Change the language packet of application, the interface must be called by signal,
     * which is emited by actions of language menu.
     */
    void installLanguage();

    /**
     * @brief openDebugPageWidget: Show a deugging window, the interface must be called by singal,
     * which is emited by actions of new debugging window menu.
     */
    void openDebugPageWidget();

    /**
     * @brief appendRemovablePage: append a page to tab widget, the interface must be called by signal,
     * which is emited by actions of new debugging page menu.
     */
    void appendRemovablePage();
private:
    Ui::SAKMainWindow *mUi;
    QTabWidget *mTabWidget;
};

#endif // MAINWINDOW_H

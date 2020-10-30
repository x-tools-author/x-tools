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
class SAKMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SAKMainWindow(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindow();

    static const QString settingKeyClearConfiguration();

    // Debug page type supported by QtSwissArmyKnife
    enum SAKEnumDebugPageType {
        DebugPageTypeTest,
#ifdef SAK_IMPORT_COM_MODULE
        DebugPageTypeCOM,
#endif
#ifdef SAK_IMPORT_HID_MODULE
        DebugPageTypeHID,
#endif
#ifdef SAK_IMPORT_USB_MODULE
        DebugPageTypeUSB,
#endif
        DebugPageTypeUdpClient,
        DebugPageTypeUdpServer,
        DebugPageTypeTCPClient,
        DebugPageTypeTCPServer,
#ifdef SAK_IMPORT_MODULE_SSLSOCKET
        DebugPageTypeSslSocketClient,
        DebugPageTypeSslSocketServer,
#endif
#ifdef SAK_IMPORT_SCTP_MODULE
        DebugPageTypeSCTPClient,
        DebugPageTypeSCTPServer,
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
        DebugPageTypeBluetoothClient,
        DebugPageTypeBluetoothServer,
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
        DebugPageTypeWebSocketClient,
        DebugPageTypeWebSocketServer
#endif
    };
    Q_ENUM(SAKEnumDebugPageType);
private:
    struct SAKDebugPageMetaInfo {
        int debugPageType;
        QMetaObject metaObject;
        QString defaultTitle;
    };
    QList<SAKDebugPageMetaInfo> mDebugPageMetaInfoList;

    QMenu *mToolsMenu;
    QMenu *mWindowsMenu;
    QAction *mTestPageAction;
    QAction *mDefaultStyleSheetAction;
    SAKUpdateManager *mUpdateManager;
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
    void appendWindowAction(QWidget *page);
    void testPageActionTriggered();
    void clearConfiguration();
    void rebootRequestion();
    void initializingMetaObject();
    void showReleaseHistoryActionDialog();
    QString tabPageName(int type);
    QWidget *debugPage(QObject *sender);
    void showQrCodeDialog();
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
    /**
     * @brief debugPageFromDebugPageType: Create a debuge page from specified type
     * @param type: Debug page type, look at the SAKEnumToolType for more information
     * @return A pointer of debug page instance
     */
    QWidget *debugPageFromDebugPageType(int type);

    /**
     * @brief debugPageTitleFromDebugPageType: Get the default name of debuge page from specified type
     * @param type: Debug page type, look at the SAKEnumToolType for more information
     * @return The default debug page name
     */
    QString debugPageTitleFromDebugPageType(int type);
private:
    Ui::SAKMainWindow *mUi;
    QTabWidget *mTabWidget;
};

#endif // MAINWINDOW_H

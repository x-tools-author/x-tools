/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMAINWINDOWDEBUGPAGESFACTORY_HH
#define SAKMAINWINDOWDEBUGPAGESFACTORY_HH

#include <QMutex>
#include <QObject>
#include <QWidget>
#include <QMetaObject>

/// @brief The class is a singleton instanc. It is thread safe.
class SAKMainWindowDebugPagesFactory : public QObject
{
    Q_OBJECT
private:
    SAKMainWindowDebugPagesFactory(QObject *parent = Q_NULLPTR);
    ~SAKMainWindowDebugPagesFactory();
public:
    static SAKMainWindowDebugPagesFactory *instance();

    /**
     * @brief debugPageFromDebugPageType: Create a debuge page from specified type
     * @param type: Debug page type, look at the SAKMainWindowDebugPageFactory::SAKEnumToolType for more information
     * @return A pointer of debug page instance
     */
    QWidget *debugPageFromDebugPageType(int type);

    /**
     * @brief debugPageTitleFromDebugPageType: Get the default name of debuge page from specified type
     * @param type: Debug page type, look at the SAKMainWindowDebugPageFactory::SAKEnumToolType for more information
     * @return The default debug page name
     */
    QString debugPageTitleFromDebugPageType(int type);

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

    static SAKMainWindowDebugPagesFactory *instancePtr;
    static QMutex *singletonMutex;
    QList<SAKDebugPageMetaInfo> mDebugPageMetaInfoList;
};

#endif // SAKMAINWINDOWDEBUGPAGEFACTORY_HH

/*****************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKDEBUGGERFACTORY_HH
#define SAKDEBUGGERFACTORY_HH

#include "base/SAKSigleton.hh"

class SAKDebuggerFactory : public QObject,
        public SAKSigleton<SAKDebuggerFactory>
{
    Q_OBJECT
    friend class SAKSigleton<SAKDebuggerFactory>;
public:
    // Debug page type supported by QtSwissArmyKnife
    enum SAKEnumDebugPageType {
#ifdef QT_DEBUG
#ifdef SAK_IMPORT_MODULE_TEST
        DebugPageTypeTest,
#endif
#endif
#ifdef SAK_IMPORT_MODULE_SERIALPORT
        DebugPageTypeCOM,
#endif
#ifdef SAK_IMPORT_HID_MODULE
        DebugPageTypeHID,
#endif
#ifdef SAK_IMPORT_MODULE_UDP
#ifdef SAK_IMPORT_MODULE_UDP_CLIENT
        DebugPageTypeUdpClient,
#endif
#ifdef SAK_IMPORT_MODULE_UDP_SERVER
        DebugPageTypeUdpServer,
#endif
#endif
#ifdef SAK_IMPORT_MODULE_TCP
#ifdef SAK_IMPORT_MODULE_TCP_CLIENT
        DebugPageTypeTCPClient,
#endif
#ifdef SAK_IMPORT_MODULE_TCP_SERVER
        DebugPageTypeTCPServer,
#endif
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
        DebugPageTypeWebSocketClient,
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_SERVER
        DebugPageTypeWebSocketServer,
#endif
#endif
#ifdef SAK_IMPORT_MODULE_SERIALBUS
        DebugPageTypeModbus
#endif
#ifdef SAK_IMPORT_MODULE_BLE_CENTRAL
        DebugPageTypeBleCentral,
#endif
#ifdef SAK_IMPORT_MODULE_MODBUS_STUDIO
        DebugPageTypeModbusStudio,
#endif
#ifdef SAK_IMPORT_MODULE_CANBUS_STUDIO
        DebugPageTypeCanStudio,
#endif
    };
    Q_ENUM(SAKEnumDebugPageType);
private:
    struct SAKDebuggerMetaObjCtx {
        int debuggerType;
        QMetaObject metaObject;
        QString debuggerName;
    };
    QVector<SAKDebuggerMetaObjCtx> mDebuggerMetaObjectContextVector;
private:
    SAKDebuggerFactory(QObject *parent = Q_NULLPTR);
public:
    QVector<SAKDebuggerMetaObjCtx> supportedDebuggersContext();
    QWidget *createDebugger(int type);
    QString debuggerName(int type);
private:
    void initializeDebuggerMetaObject();
};

#define sakDebuggerFactor SAKDebuggerFactory::instance()
typedef SAKDebuggerFactory::SAKEnumDebugPageType SAKEnumDebugPageType;

#endif // SAKDEBUGGERFACTORY_HH

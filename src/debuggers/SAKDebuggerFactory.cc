/*****************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QWidget>
#include <QMetaObject>

#include "SAKApplication.hh"
#include "SAKDebuggerFactory.hh"

// Debugging pages
#ifdef QT_DEBUG
#ifdef SAK_IMPORT_MODULE_TEST
#include "SAKTestDebugger.hh"
#endif
#endif
#ifdef SAK_IMPORT_MODULE_SERIALBUS
#include "SAKModbusDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_MODBUS
#include "SAKModbusDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_UDP
#ifdef SAK_IMPORT_MODULE_UDP_CLIENT
#include "SAKUdpClientDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_UDP_SERVER
#include "SAKUdpServerDebugger.hh"
#endif
#endif
#ifdef SAK_IMPORT_MODULE_TCP
#ifdef SAK_IMPORT_MODULE_TCP_CLIENT
#include "SAKTcpClientDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_TCP_SERVER
#include "SAKTcpServerDebugger.hh"
#endif
#endif
#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include "SAKSerialPortDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
#include "SAKWebSocketClientDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_SERVER
#include "SAKWebSocketServerDebugger.hh"
#endif
#endif
#ifdef SAK_IMPORT_MODULE_BLE_CENTRAL
#include "SAKBleCentralDebugger.hh"
#endif
#ifdef SAK_IMPORT_MODULE_CANBUS
#include "SAKCanBusDebugger.hh"
#endif

SAKDebuggerFactory::SAKDebuggerFactory(QObject *parent)
    : QObject(parent)
    , SAKSigleton<SAKDebuggerFactory>("SAK.Debuggers.Factory")
{
    initializeDebuggerMetaObject();
}

QVector<SAKDebuggerFactory::SAKDebuggerMetaObjCtx>
SAKDebuggerFactory::supportedDebuggersContext()
{
    return mDebuggerMetaObjectContextVector;
}

QWidget *SAKDebuggerFactory::createDebugger(int type)
{
    QWidget *widget = Q_NULLPTR;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (SAKDebuggerMetaObjCtx &var : mDebuggerMetaObjectContextVector) {
        if (var.debuggerType == type){
            for (int i = 0; i < metaEnum.keyCount(); i++){
                if (var.debuggerType == metaEnum.value(i)){
                    QObject *obj = var.metaObject.newInstance(
                                Q_ARG(QSettings*, sakApp->settings()),
                                Q_ARG(QString, QString(metaEnum.key(i))),
                                Q_ARG(QSqlDatabase*, sakApp->sqlDatabase()),
                                Q_ARG(QWidget*, Q_NULLPTR)
                                );
                    widget = qobject_cast<QWidget*>(obj);
                    if (widget) {
                        widget->setWindowTitle(debuggerName(type));
                    } else {
                        QString log = "Can not instance debugger("
                                + QString::number(type) + ")";
                        printLog(QtWarningMsg, log);
                    }
                    break;
                }
            }
        }
    }

    return widget;
}

QString SAKDebuggerFactory::debuggerName(int type)
{
    QString title = QString("UnknownDebugPage");
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (SAKDebuggerMetaObjCtx &var : mDebuggerMetaObjectContextVector) {
        if (var.debuggerType == type) {
            for (int i = 0; i < metaEnum.keyCount(); i++) {
                if (var.debuggerType == metaEnum.value(i)) {
                    title = var.debuggerName;
                    break;
                }
            }
        }
    }

    if (title.isEmpty()){
        QString log = "Unknown debug page type:" + QString::number(type);
        printLog(QtWarningMsg, log);
        Q_ASSERT_X(false, __FUNCTION__, "Unknown debug page type!");
    }

    return title;
}

void SAKDebuggerFactory::initializeDebuggerMetaObject()
{
#ifdef QT_DEBUG
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeTest,
                                      SAKTestDebugger::staticMetaObject,
                                      tr("Test")});
#endif
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeCOM,
                                      SAKSerialPortDebugger::staticMetaObject,
                                      tr("COM")
                                  });
#endif
#ifdef SAK_IMPORT_MODULE_UDP
#ifdef SAK_IMPORT_MODULE_UDP_CLIENT
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeUdpClient,
                                      SAKUdpClientDebugger::staticMetaObject,
                                      tr("UdpClient")});
#endif
#ifdef SAK_IMPORT_MODULE_UDP_SERVER
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeUdpServer,
                                      SAKUdpServerDebugger::staticMetaObject,
                                      tr("UdpServer")});
#endif
#endif
#ifdef SAK_IMPORT_MODULE_TCP
#ifdef SAK_IMPORT_MODULE_TCP_CLIENT
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeTCPClient,
                                      SAKTcpClientDebugger::staticMetaObject,
                                      tr("TcpClient")});
#endif
#ifdef SAK_IMPORT_MODULE_TCP_SERVER
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeTCPServer,
                                      SAKTcpServerDebugger::staticMetaObject,
                                      tr("TcpServer")});
#endif
#endif
#ifdef SAK_IMPORT_MODULE_BLE_CENTRAL
    mDebugPageMetaInfoList.append(SAKDebuggerMetaObjectContext{
                                      DebugPageTypeBleCentral,
                                      SAKBleCentralDebugger::staticMetaObject,
                                      tr("BleCentral")});
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
    mDebuggerMetaObjectContextVector.append(
                SAKDebuggerMetaObjCtx{
                    DebugPageTypeWebSocketClient,
                    SAKWebSocketClientDebugger::staticMetaObject,
                    tr("WsClient")});
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_SERVER
    mDebuggerMetaObjectContextVector.append(
                SAKDebuggerMetaObjCtx{
                    DebugPageTypeWebSocketServer,
                    SAKWebSocketServerDebugger::staticMetaObject,
                    tr("WsServer")});
#endif
#endif
#ifdef SAK_IMPORT_MODULE_SERIALBUS
    mDebugPageMetaInfoList.append(SAKDebuggerMetaObjectContext{
                                      DebugPageTypeModbus,
                                      SAKModbusDebugger::staticMetaObject,
                                      tr("Modbus")});
#endif
#ifdef SAK_IMPORT_MODULE_MODBUS
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeModbusStudio,
                                      SAKModbusDebugger::staticMetaObject,
                                      tr("Modbus")});
#endif
#ifdef SAK_IMPORT_MODULE_CANBUS
    mDebuggerMetaObjectContextVector.append(SAKDebuggerMetaObjCtx{
                                      DebugPageTypeCanStudio,
                                      SAKCanBusDebugger::staticMetaObject,
                                      tr("CAN")});
#endif
}

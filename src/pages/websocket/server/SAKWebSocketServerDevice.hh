/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWEBSOCKETSERVERDEVICE_HH
#define SAKWEBSOCKETSERVERDEVICE_HH

#include <QThread>
#include <QWebSocket>
#include <QWebSocketServer>

#include "SAKDebugPageDevice.hh"

class SAKWebSocketServerDebugPage;
class SAKWebSocketServerDeviceController;
/// @brief web socket服务器设备
class SAKWebSocketServerDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKWebSocketServerDevice(SAKWebSocketServerDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run();    
private:
    QString mLocalHost;
    quint16 mLocalPort;
    bool mEnableCustomLocalSetting;
    QString mServerHost;
    quint16 mServerPort;
    SAKWebSocketServerDebugPage *mDebugPage;
    QWebSocketServer *mWebSocketServer;
private:
    void innerReadBytes(QWebSocket *socket, QByteArray bytes, SAKWebSocketServerDeviceController *deviceController);
    void innerWriteBytes(QWebSocket *socket, QByteArray bytes, SAKWebSocketServerDeviceController *deviceController);
};

#endif

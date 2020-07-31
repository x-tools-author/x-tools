/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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

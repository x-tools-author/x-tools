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

class SAKWebSocketServerDebugger;
class SAKWebSocketServerController;
/// @brief Web socket server device
class SAKWebSocketServerDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKWebSocketServerDevice(SAKWebSocketServerDebugger *debugPage, QObject *parent = Q_NULLPTR);
private:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    bool checkSomething(QString &errorString) final;
    void close() final;
    void free() final;
signals:
    void addClient(QString host, quint16 port, QWebSocket *socket);
    void removeClient(QWebSocket *socket);
    void clearClient();
private:
    QWebSocketServer *mWebSocketServer;
    SAKWebSocketServerDebugger *mDebugPage;
    SAKWebSocketServerController *mDeviceController;
    QList<QWebSocket*> mClientList;
private:
    void readBytesActually(QWebSocket *socket, QByteArray bytes);
};

#endif

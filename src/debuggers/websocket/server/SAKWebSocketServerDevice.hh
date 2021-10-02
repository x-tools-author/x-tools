/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKDebuggerDevice.hh"

class SAKWebSocketServerDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKWebSocketServerDevice(QSettings *settings,
                             const QString &settingsGroup,
                             QWidget *uiParent,
                             QObject *parent = Q_NULLPTR);
protected:
    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QWebSocketServer *mWebSocketServer;
    QList<QWebSocket*> mClientList;
private:
    void readBytesActually(QWebSocket *socket, QByteArray bytes);
signals:
    void addClient(QString host, quint16 port, QWebSocket *socket);
    void removeClient(QWebSocket *socket);
    void clearClient();
};

#endif

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWebSocketServer>

#include "socketserver.h"

class WebSocketServer : public SocketServer
{
    Q_OBJECT
public:
    explicit WebSocketServer(QObject *parent = nullptr);
    ~WebSocketServer() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

private:
    QWebSocketServer *m_webSocketServer{nullptr};
    QList<QWebSocket *> m_sockets;

private:
    void setupSocket(QWebSocket *socket);
    void writeBytes(QWebSocket *socket, const QByteArray &bytes);

    void onTextMessageReceived(QWebSocket *socket, const QString &message);
    void onBinaryMessageReceived(QWebSocket *socket, const QByteArray &message);
};

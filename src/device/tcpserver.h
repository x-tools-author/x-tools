/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTcpServer>

#include "socketserver.h"

class TcpServer : public SocketServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

    void disconnectAllClients() override;

private:
    QTcpServer *m_tcpServer{nullptr};
    QList<QTcpSocket *> m_sockets;

private:
    void setupClient(QTcpSocket *socket);
    void writeBytes(QTcpSocket *socket, const QByteArray &bytes);
    void readBytes(QTcpSocket *socket);
    void removeSocket(QTcpSocket *socket);
};

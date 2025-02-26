/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QUdpSocket>

#include "socketserver.h"

class UdpServer : public SocketServer
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);
    ~UdpServer() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

    void disconnectAllClients() override;

private:
    QUdpSocket *m_udpSocket{nullptr};

private:
    void readPendingDatagrams();
    void writeDatagram(const QByteArray &bytes, const QString &flag);
};

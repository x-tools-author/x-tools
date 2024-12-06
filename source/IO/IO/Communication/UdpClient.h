/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QUdpSocket>

#include "SocketClient.h"

namespace xTools {

class UdpClient : public SocketClient
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeBytes(const QByteArray &bytes) override;

private:
    QUdpSocket *m_udpSocket{nullptr};

private:
    void readPendingDatagrams();
    void writeDatagram(const QByteArray &bytes, const QString &ip, quint16 port);
};

} // namespace xTools

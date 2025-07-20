/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "device.h"

#include <QNetworkInterface>
#include <QUdpSocket>
#include <QVariantMap>

struct UdpBroadcastParameters
{
    QString address;
    int port;
};

UdpBroadcastParameters variantMap2UdpBroadcastParameters(const QVariantMap &obj);
QVariantMap udpBroadcastParameters2variantMap(const UdpBroadcastParameters &params);

class UdpBroadcast : public Device
{
    Q_OBJECT
public:
    explicit UdpBroadcast(QObject *parent = nullptr);
    ~UdpBroadcast() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeActually(const QByteArray &bytes) override;

private:
    QUdpSocket *m_udpSocket{nullptr};
    UdpBroadcastParameters m_parameters;

private:
    void readPendingDatagrams();
};

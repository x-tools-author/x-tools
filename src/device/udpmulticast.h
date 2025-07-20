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

struct UdpMulticastParameters
{
    QString address;
    int port;
    QString name;
    bool loopback;
};

UdpMulticastParameters variantMap2UdpMulticastParameters(const QVariantMap &obj);
QVariantMap udpMulticastParameters2variantMap(const UdpMulticastParameters &params);

class UdpMulticast : public Device
{
    Q_OBJECT
public:
    explicit UdpMulticast(QObject *parent = nullptr);
    ~UdpMulticast() override;

    QObject *initDevice() override;
    void deinitDevice() override;
    void writeActually(const QByteArray &bytes) override;

private:
    QUdpSocket *m_udpSocket{nullptr};
    UdpMulticastParameters m_parameters;

private:
    void readPendingDatagrams();
};

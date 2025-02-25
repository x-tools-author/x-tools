/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ipscanner.h"

#include <QHostAddress>
#include <QNetworkInterface>

namespace xTools {

IpScanner::IpScanner(QObject* parent)
    : QObject{parent}
{
    refresh();
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(1 * 1000);
    m_refreshTimer->setSingleShot(true);
    connect(m_refreshTimer, &QTimer::timeout, this, [=]() {
        refresh();
        m_refreshTimer->start();
    });

    m_refreshTimer->start();
}

void IpScanner::refresh()
{
    QStringList ipv4List, ipv6List;
    auto addresses = QNetworkInterface::allAddresses();
    for (auto& address : addresses) {
        auto str = address.toString();

        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipv4List.append(str);
        } else if (address.protocol() == QAbstractSocket::IPv6Protocol) {
            ipv6List.append(str);
        }
    }

    if (m_enableAutoRefresh) {
        QStringList temp;
        if (m_enableIpV4) {
            temp.append(ipv4List);
        }

        if (m_enableIpV6) {
            temp.append(ipv6List);
        }

        if (temp != m_ipList) {
            m_ipList = temp;
            emit ipListChanged();
        }
    }
}

QStringList IpScanner::ipList()
{
    return m_ipList;
}

bool IpScanner::enableIpV4()
{
    return m_enableIpV4;
}

void IpScanner::setEnableIpV4(bool enable)
{
    m_enableIpV4 = enable;
    emit enableIpV4Changed();
}

bool IpScanner::enableIpV6()
{
    return m_enableIpV4;
}

void IpScanner::setEnableIpV6(bool enable)
{
    m_enableIpV6 = enable;
    emit enableIpV6Changed();
}

bool IpScanner::enableAutoRefresh()
{
    return m_enableAutoRefresh;
}

void IpScanner::setEnableAutoRefresh(bool enable)
{
    m_enableAutoRefresh = enable;
    emit enableAutoRefreshChanged();
}

} // namespace xTools

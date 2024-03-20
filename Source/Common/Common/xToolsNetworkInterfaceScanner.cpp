/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsNetworkInterfaceScanner.h"

#include <QHostAddress>
#include <QNetworkInterface>

xToolsNetworkInterfaceScanner::xToolsNetworkInterfaceScanner(QObject* parent)
    : QObject{parent}
{
    refresh();
    mRefreshTimer = new QTimer(this);
    mRefreshTimer->setInterval(1 * 1000);
    mRefreshTimer->setSingleShot(true);
    connect(mRefreshTimer, &QTimer::timeout, this, [=]() {
        refresh();
        mRefreshTimer->start();
    });

    mRefreshTimer->start();
}

void xToolsNetworkInterfaceScanner::refresh()
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

    if (mEnableAutoRefresh) {
        QStringList temp;
        if (mEnableIpV4) {
            temp.append(ipv4List);
        }

        if (mEnableIpV6) {
            temp.append(ipv6List);
        }

        if (temp != mIpList) {
            mIpList = temp;
            emit ipListChanged();
        }
    }
}

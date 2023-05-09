/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QHostAddress>
#include <QNetworkInterface>

#include "EDNetworkInterface.hpp"

EDNetworkInterface::EDNetworkInterface(QObject *parent)
    : QObject{parent}
{
    refresh();
    mRefreshTimer = new QTimer(this);
    mRefreshTimer->setInterval(1*1000);
    mRefreshTimer->setSingleShot(true);
    connect(mRefreshTimer, &QTimer::timeout, this, [=](){
        refresh();
        mRefreshTimer->start();
    });

    mRefreshTimer->start();
}

void EDNetworkInterface::refresh()
{
    QStringList ipv4List, ipv6List;
    auto addresses = QNetworkInterface::allAddresses();
    for (auto &address : addresses) {
        auto str = address.toString();
        if (address.protocol() == QHostAddress::IPv4Protocol) {
            ipv4List.append(str);
        } else if (address.protocol() == QHostAddress::IPv6Protocol) {
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

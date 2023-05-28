/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QBluetoothDeviceInfo>
#include "EDBle.hpp"

EDBle::EDBle(QObject *parent)
    : QObject{parent}
{
    mDiscover = new QBluetoothDeviceDiscoveryAgent();
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &EDBle::onFinished);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &EDBle::onErrorOccurred);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &EDBle::onDeviceDiscovered);

    refresh();
}

EDBle::~EDBle()
{
    mDiscover->deleteLater();
}

QVariant EDBle::bleInfo(int index)
{
    auto infos = mDiscover->discoveredDevices();
    if (index >= 0 && index < infos.length()) {
        return QVariant::fromValue<QBluetoothDeviceInfo>(infos.at(index));
    }

    return QVariant();
}

void EDBle::refresh()
{
    mIsDiscovering = true;
    //emit isDiscoveringChanged();
    mDiscover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void EDBle::onFinished()
{
    mIsDiscovering = false;
    //emit isDiscoveringChanged();
    if (mEnableRefresh) {
        auto devices = mDiscover->discoveredDevices();
        QStringList temp;
        for (auto &device : devices) {
            if (mFiltter.isEmpty()) {
                temp.append(device.name());
            } else {
                if (temp.contains(mFiltter)) {
                    temp.append(device.name());
                }
            }
        }

        if (mNames != temp) {
            mNames = temp;
            emit namesChanged();
        }

        int interval = mTimeout < 5 ? 5 : mTimeout;
        QTimer::singleShot(interval*1000, this, [=](){refresh();});
    }
}

void EDBle::onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    mIsDiscovering = false;
    //emit isDiscoveringChanged();
    Q_UNUSED(error);
    qWarning() << "QBluetoothDeviceDiscoveryAgent error:" << mDiscover->errorString();
}

void EDBle::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    Q_UNUSED(info);
}

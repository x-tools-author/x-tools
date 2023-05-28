/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QBluetoothDeviceInfo>
#include "SAKBleScanner.hh"

SAKBleScanner::SAKBleScanner(QObject *parent)
    : QObject{parent}
{
    mDiscover = new QBluetoothDeviceDiscoveryAgent();
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &SAKBleScanner::onFinished);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &SAKBleScanner::onErrorOccurred);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &SAKBleScanner::onDeviceDiscovered);

    refresh();
}

SAKBleScanner::~SAKBleScanner()
{
    mDiscover->deleteLater();
}

QVariant SAKBleScanner::bleInfo(int index)
{
    auto infos = mDiscover->discoveredDevices();
    if (index >= 0 && index < infos.length()) {
        return QVariant::fromValue<QBluetoothDeviceInfo>(infos.at(index));
    }

    return QVariant();
}

void SAKBleScanner::refresh()
{
    mIsDiscovering = true;
    //emit isDiscoveringChanged();
    mDiscover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void SAKBleScanner::onFinished()
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

void SAKBleScanner::onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    mIsDiscovering = false;
    //emit isDiscoveringChanged();
    Q_UNUSED(error);
    qWarning() << "QBluetoothDeviceDiscoveryAgent error:" << mDiscover->errorString();
}

void SAKBleScanner::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    Q_UNUSED(info);
}

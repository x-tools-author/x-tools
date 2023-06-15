/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QDebug>
#include <QtGlobal>
#include <QBluetoothDeviceInfo>

#include "SAKBleScanner.hh"

SAKBleScanner::SAKBleScanner(QObject *parent)
    : QObject{parent}
{
    mDiscover = new QBluetoothDeviceDiscoveryAgent();
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &SAKBleScanner::onFinished);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &SAKBleScanner::onErrorOccurred);
#else
#if 1
    connect(mDiscover, static_cast<void(QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
            this, &SAKBleScanner::onErrorOccurred);
#else
    connect(mDiscover, qOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &SAKBleScanner::onErrorOccurred);
#endif
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    mDiscover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
#endif
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

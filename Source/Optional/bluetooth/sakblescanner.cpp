/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakblescanner.h"

#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QtGlobal>

#define BLE_ERR_SIG void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)

SAKBleScanner::SAKBleScanner(QObject* parent)
    : QThread(parent)
    , m_discover(Q_NULLPTR)
{}

SAKBleScanner::~SAKBleScanner() {}

void SAKBleScanner::startDiscover()
{
    start();
}

void SAKBleScanner::stopDiscover()
{
    exit();
}

bool SAKBleScanner::isActive()
{
    return isRunning();
}

QVariant SAKBleScanner::deviceInfo(int index)
{
    m_deviceInfoListMutex.lock();
    if (index >= 0 && index < m_deviceInfoList.length()) {
        QBluetoothDeviceInfo info = m_deviceInfoList.at(index);
        return QVariant::fromValue<QBluetoothDeviceInfo>(info);
    }
    m_deviceInfoListMutex.unlock();

    return QVariant();
}

QString SAKBleScanner::deviceName(const QVariant& deviceInfo)
{
    auto cookedInfo = deviceInfo.value<QBluetoothDeviceInfo>();
    return cookedInfo.name();
}

void SAKBleScanner::run()
{
    m_discover = new QBluetoothDeviceDiscoveryAgent();
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::finished,
            this,
            &SAKBleScanner::onDiscoveryFinished);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this,
            &SAKBleScanner::onDiscoveryErrorOccurred);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this,
            &SAKBleScanner::onDiscoveryDeviceDiscovered);

    // 10s-1minute
    int interval = m_timeoutInterval < 10 ? 10 : m_timeoutInterval;
    interval = interval > 120 ? 120 : interval;
    m_discover->setLowEnergyDiscoveryTimeout(interval * 1000);

    m_deviceInfoListMutex.lock();
    m_deviceInfoList.clear();
    m_deviceInfoListMutex.unlock();
    m_discover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    exec();
}

void SAKBleScanner::onDiscoveryFinished()
{
    emit devicesInfoListChanged();
    exit();
}

void SAKBleScanner::onDiscoveryErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    Q_UNUSED(error);
    qWarning() << "QBluetoothDeviceDiscoveryAgent error:" << m_discover->errorString();
    exit();
    emit errorOccurred(m_discover->errorString());
}

void SAKBleScanner::onDiscoveryDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    const QString name = info.name();
    qInfo() << "new ble device:" << name;

    if (!m_nameFiltter.isEmpty()) {
        if (!name.contains(m_nameFiltter)) {
            qInfo() << "device is ignored:" << name;
            return;
        }
    }

    m_deviceInfoListMutex.lock();
    m_deviceInfoList.append(info);
    m_deviceInfoListMutex.unlock();
    emit deviceDiscovered(info);
}

QVariantList SAKBleScanner::devicesInfoList()
{
    QVariantList list;
    m_deviceInfoListMutex.lock();
    for (auto& info : m_deviceInfoList) {
        list.append(QVariant::fromValue(info));
    }
    m_deviceInfoListMutex.unlock();

    return list;
}

int SAKBleScanner::timeoutInterval()
{
    return m_timeoutInterval;
}

void SAKBleScanner::setTimeoutInterval(int interval)
{
    m_timeoutInterval = interval;
    emit timeoutIntervalChanged();
}

QString SAKBleScanner::namefiltter()
{
    return m_nameFiltter;
}

void SAKBleScanner::setNameFiltter(const QString& flag)
{
    m_nameFiltter = flag;
    emit filtterNameChanged();
}

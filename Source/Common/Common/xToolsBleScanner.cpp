/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBleScanner.h"

#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QtGlobal>

#define BLE_ERR_SIG void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)

xToolsBleScanner::xToolsBleScanner(QObject* parent)
    : QThread(parent)
    , m_discover(Q_NULLPTR)
{}

xToolsBleScanner::~xToolsBleScanner() {}

void xToolsBleScanner::startDiscover()
{
    start();
}

void xToolsBleScanner::stopDiscover()
{
    exit();
}

bool xToolsBleScanner::isActive()
{
    return isRunning();
}

QVariant xToolsBleScanner::deviceInfo(int index)
{
    m_deviceInfoListMutex.lock();
    if (index >= 0 && index < m_deviceInfoList.length()) {
        QBluetoothDeviceInfo info = m_deviceInfoList.at(index);
        return QVariant::fromValue<QBluetoothDeviceInfo>(info);
    }
    m_deviceInfoListMutex.unlock();

    return QVariant();
}

QString xToolsBleScanner::deviceName(const QVariant& deviceInfo)
{
    auto cookedInfo = deviceInfo.value<QBluetoothDeviceInfo>();
    return cookedInfo.name();
}

void xToolsBleScanner::run()
{
    m_discover = new QBluetoothDeviceDiscoveryAgent();
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::finished,
            this,
            &xToolsBleScanner::onDiscoveryFinished);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this,
            &xToolsBleScanner::onDiscoveryErrorOccurred);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this,
            &xToolsBleScanner::onDiscoveryDeviceDiscovered);

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

void xToolsBleScanner::onDiscoveryFinished()
{
    emit devicesInfoListChanged();
    exit();
}

void xToolsBleScanner::onDiscoveryErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    Q_UNUSED(error);
    qWarning() << "QBluetoothDeviceDiscoveryAgent error:" << m_discover->errorString();
    exit();
    emit errorOccurred(m_discover->errorString());
}

void xToolsBleScanner::onDiscoveryDeviceDiscovered(const QBluetoothDeviceInfo& info)
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

QVariantList xToolsBleScanner::devicesInfoList()
{
    QVariantList list;
    m_deviceInfoListMutex.lock();
    for (auto& info : m_deviceInfoList) {
        list.append(QVariant::fromValue(info));
    }
    m_deviceInfoListMutex.unlock();

    return list;
}

int xToolsBleScanner::timeoutInterval()
{
    return m_timeoutInterval;
}

void xToolsBleScanner::setTimeoutInterval(int interval)
{
    m_timeoutInterval = interval;
    emit timeoutIntervalChanged();
}

QString xToolsBleScanner::namefiltter()
{
    return m_nameFiltter;
}

void xToolsBleScanner::setNameFiltter(const QString& flag)
{
    m_nameFiltter = flag;
    emit filtterNameChanged();
}

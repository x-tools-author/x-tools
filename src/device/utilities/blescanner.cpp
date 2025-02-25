/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "blescanner.h"

#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QtGlobal>

namespace xTools {

BleScanner::BleScanner(QObject* parent)
    : QThread(parent)
    , m_discover(Q_NULLPTR)
{
    connect(this, &BleScanner::started, this, [this]() {
        this->m_isBusy = true;
        emit isBusyChanged();
    });
    connect(this, &BleScanner::finished, this, [this]() {
        this->m_isBusy = false;
        emit isBusyChanged();
    });
}

BleScanner::~BleScanner()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

void BleScanner::stop()
{
    exit();
    wait();
}

void BleScanner::run()
{
    m_discover = new QBluetoothDeviceDiscoveryAgent();

    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::finished,
            this,
            &BleScanner::onDiscoveryFinished);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this,
            &BleScanner::onDiscoveryErrorOccurred);
    connect(m_discover,
            &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this,
            &BleScanner::onDiscoveryDeviceDiscovered);

    m_discover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    exec();

    m_discover->stop();
    m_discover->deleteLater();
    m_discover = Q_NULLPTR;
}

void BleScanner::onDiscoveryFinished()
{
    exit();
    wait();
}

void BleScanner::onDiscoveryErrorOccurred()
{
    qWarning() << "QBluetoothDeviceDiscoveryAgent error:" << m_discover->errorString();
    exit();
    emit errorOccurred(m_discover->errorString());
}

void BleScanner::onDiscoveryDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    const QString name = info.name();
    qInfo() << "New BLE device:" << name << QVariant::fromValue(info);

    emit deviceDiscovered(info);
    emit deviceDiscoveredForQml(name, QVariant::fromValue(info));
}

} // namespace xTools

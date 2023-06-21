/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDebug>
#include <QtGlobal>
#include <QBluetoothDeviceInfo>

#include "SAKBleScanner.hh"

#define BLE_ERR_SIG void(QBluetoothDeviceDiscoveryAgent::*)\
(QBluetoothDeviceDiscoveryAgent::Error)

SAKBleScanner::SAKBleScanner(QObject *parent)
    : QObject(parent)
    , mDiscover(Q_NULLPTR)
    , mIsFirstTime(true)
    , mAutoRestart(true)
{
    mDiscover = new QBluetoothDeviceDiscoveryAgent(this);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &SAKBleScanner::onFinished);
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &SAKBleScanner::finished);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &SAKBleScanner::onErrorOccurred);
#else
    connect(mDiscover,
            static_cast<BLE_ERR_SIG>(&QBluetoothDeviceDiscoveryAgent::error),
            this, &SAKBleScanner::onErrorOccurred);
#endif
    connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &SAKBleScanner::onDeviceDiscovered);
}

SAKBleScanner::~SAKBleScanner()
{
    mDiscover->stop();
    mDiscover->deleteLater();
}

void SAKBleScanner::startDiscover()
{
    mAutoRestart = true;

    // 10s-1minute
    int interval = mTimeoutInterval < 10 ? 10 : mTimeoutInterval;
    interval = interval > 120 ? 120 : interval;
    mDiscover->setLowEnergyDiscoveryTimeout(interval*1000);

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    if (mDiscover->isActive()) {
        return;
    }
    mDiscover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
#endif

    mDeviceInfoListTemp.clear();
}

void SAKBleScanner::stopDiscover()
{
    mAutoRestart = false;
    mDiscover->stop();
}

bool SAKBleScanner::isActive()
{
    return mDiscover->isActive();
}

QVariant SAKBleScanner::deviceInfo(int index)
{
    if (index >= 0 && index < mDeviceInfoList.length()) {
        QBluetoothDeviceInfo info = mDeviceInfoList.at(index);
        return QVariant::fromValue<QBluetoothDeviceInfo>(info);
    }

    return QVariant();
}

QString SAKBleScanner::deviceName(const QVariant &deviceInfo)
{
    auto cookedInfo = deviceInfo.value<QBluetoothDeviceInfo>();
    return cookedInfo.name();
}

void SAKBleScanner::onFinished()
{
    if (mIsFirstTime) {
        mIsFirstTime = false;
        return;
    }

    if (mEnableRefresh) {
        if (mDeviceInfoList != mDeviceInfoListTemp) {
            mDeviceInfoList = mDeviceInfoListTemp;
            emit devicesInfoListChanged();
        }
    }

    mDeviceInfoListTemp.clear();
    if (mAutoRestart) {
        startDiscover();
    }
}

void SAKBleScanner::onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    Q_UNUSED(error);
    qCWarning(mLoggingCategory) << "QBluetoothDeviceDiscoveryAgent error:"
                                << mDiscover->errorString();

    if (mAutoRestart) {
        startDiscover();
    }
}

void SAKBleScanner::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    const QString name = info.name();
    qCInfo(mLoggingCategory) << "new ble device:" << name;

    if (!mNameFiltter.isEmpty()) {
        if (!name.contains(mNameFiltter)) {
            qCInfo(mLoggingCategory) << "device is ignored:" << name;
            return;
        }
    }

    if (mIsFirstTime) {
        if (mEnableRefresh) {
            mDeviceInfoList.append(info);
            emit devicesInfoListChanged();
        }
    }

    mDeviceInfoListTemp.append(info);
}

QVariantList SAKBleScanner::devicesInfoList()
{
    QVariantList list;
    for (auto &info : mDeviceInfoList) {
        list.append(QVariant::fromValue(info));
    }

    return list;
}

bool SAKBleScanner::enableRefresh()
{
    return mEnableRefresh;
}

void SAKBleScanner::setEnableRefresh(bool enable)
{
    mEnableRefresh = enable;
    emit enableRefreshChanged();
}

int SAKBleScanner::timeoutInterval()
{
    return mTimeoutInterval;
}

void SAKBleScanner::setTimeoutInterval(int interval)
{
    mTimeoutInterval = interval;
    emit timeoutIntervalChanged();
}

QString SAKBleScanner::namefiltter()
{
    return mNameFiltter;
}

void SAKBleScanner::setNameFiltter(const QString &flag)
{
    mNameFiltter = flag;
    emit filtterNameChanged();
}

/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "SAKBleScanner.h"

#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QtGlobal>

#define BLE_ERR_SIG                         \
  void (QBluetoothDeviceDiscoveryAgent::*)( \
      QBluetoothDeviceDiscoveryAgent::Error)

SAKBleScanner::SAKBleScanner(QObject *parent)
    : QThread(parent), mDiscover(Q_NULLPTR) {}

SAKBleScanner::~SAKBleScanner() {}

void SAKBleScanner::startDiscover() { start(); }

void SAKBleScanner::stopDiscover() { exit(); }

bool SAKBleScanner::isActive() { return isRunning(); }

QVariant SAKBleScanner::deviceInfo(int index) {
  mDeviceInfoListMutex.lock();
  if (index >= 0 && index < mDeviceInfoList.length()) {
    QBluetoothDeviceInfo info = mDeviceInfoList.at(index);
    return QVariant::fromValue<QBluetoothDeviceInfo>(info);
  }
  mDeviceInfoListMutex.unlock();

  return QVariant();
}

QString SAKBleScanner::deviceName(const QVariant &deviceInfo) {
  auto cookedInfo = deviceInfo.value<QBluetoothDeviceInfo>();
  return cookedInfo.name();
}

void SAKBleScanner::run() {
  mDiscover = new QBluetoothDeviceDiscoveryAgent();
  connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::finished, this,
          &SAKBleScanner::onDiscoveryFinished);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
  connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this,
          &SAKBleScanner::onDiscoveryErrorOccurred);
#else
  connect(mDiscover,
          static_cast<BLE_ERR_SIG>(&QBluetoothDeviceDiscoveryAgent::error),
          this, &SAKBleScanner::onDiscoveryErrorOccurred);
#endif
  connect(mDiscover, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
          &SAKBleScanner::onDiscoveryDeviceDiscovered);

  // 10s-1minute
  int interval = mTimeoutInterval < 10 ? 10 : mTimeoutInterval;
  interval = interval > 120 ? 120 : interval;
  mDiscover->setLowEnergyDiscoveryTimeout(interval * 1000);

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
  mDeviceInfoListMutex.lock();
  mDeviceInfoList.clear();
  mDeviceInfoListMutex.unlock();
  mDiscover->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
#endif
  exec();
}

void SAKBleScanner::onDiscoveryFinished() {
  emit devicesInfoListChanged();
  exit();
}

void SAKBleScanner::onDiscoveryErrorOccurred(
    QBluetoothDeviceDiscoveryAgent::Error error) {
  Q_UNUSED(error);
  qCWarning(mLoggingCategory)
      << "QBluetoothDeviceDiscoveryAgent error:" << mDiscover->errorString();
  exit();
  emit errorOccurred(mDiscover->errorString());
}

void SAKBleScanner::onDiscoveryDeviceDiscovered(
    const QBluetoothDeviceInfo &info) {
  const QString name = info.name();
  qCInfo(mLoggingCategory) << "new ble device:" << name;

  if (!mNameFiltter.isEmpty()) {
    if (!name.contains(mNameFiltter)) {
      qCInfo(mLoggingCategory) << "device is ignored:" << name;
      return;
    }
  }

  mDeviceInfoListMutex.lock();
  mDeviceInfoList.append(info);
  mDeviceInfoListMutex.unlock();
  emit deviceDiscovered(info);
}

QVariantList SAKBleScanner::devicesInfoList() {
  QVariantList list;
  mDeviceInfoListMutex.lock();
  for (auto &info : mDeviceInfoList) {
    list.append(QVariant::fromValue(info));
  }
  mDeviceInfoListMutex.unlock();

  return list;
}

int SAKBleScanner::timeoutInterval() { return mTimeoutInterval; }

void SAKBleScanner::setTimeoutInterval(int interval) {
  mTimeoutInterval = interval;
  emit timeoutIntervalChanged();
}

QString SAKBleScanner::namefiltter() { return mNameFiltter; }

void SAKBleScanner::setNameFiltter(const QString &flag) {
  mNameFiltter = flag;
  emit filtterNameChanged();
}

/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMessageBox>
#include <QBluetoothDeviceInfo>
#include "SAKBluetoothDeviceInfoComboBox.hh"

SAKBluetoothDeviceInfoComboBox::SAKBluetoothDeviceInfoComboBox(QWidget *parent)
    : SAKComboBox(parent)
    , mScanner(Q_NULLPTR)
{
    mScanner = new SAKBleScanner(this);
    connect(mScanner, &SAKBleScanner::finished,
            this, &SAKBluetoothDeviceInfoComboBox::onFinished);
    connect(mScanner, &SAKBleScanner::started,
            this, &SAKBluetoothDeviceInfoComboBox::started);
    connect(mScanner, &SAKBleScanner::deviceDiscovered,
            this, &SAKBluetoothDeviceInfoComboBox::onDeviceDiscovered);
    connect(mScanner, &SAKBleScanner::errorOccurred,
            this, &SAKBluetoothDeviceInfoComboBox::onErrorOccurred);
}

SAKBluetoothDeviceInfoComboBox::~SAKBluetoothDeviceInfoComboBox()
{
    mScanner->stopDiscover();
}

void SAKBluetoothDeviceInfoComboBox::startDiscover()
{
    clear();
    mScanner->startDiscover();
}

void SAKBluetoothDeviceInfoComboBox::stopDiscover()
{
    mScanner->stopDiscover();
}

bool SAKBluetoothDeviceInfoComboBox::isActive()
{
    return mScanner->isActive();
}

void SAKBluetoothDeviceInfoComboBox::setTimeoutInterval(int interval)
{
    mScanner->setTimeoutInterval(interval);
}

void SAKBluetoothDeviceInfoComboBox::setNameFiltter(const QString &filtter)
{
    mScanner->setNameFiltter(filtter);
}

void SAKBluetoothDeviceInfoComboBox::changeEvent(QEvent *event)
{
    SAKComboBox::changeEvent(event);
    if ((event->type() == QEvent::EnabledChange) && isEnabled()) {
        onFinished();
    }
}

void SAKBluetoothDeviceInfoComboBox::onFinished()
{
    if (!isEnabled()) {
        return;
    }

    clear();
    auto infos = mScanner->devicesInfoList();
    for (auto &info : infos) {
        QString name = mScanner->deviceName(info);
        addItem(name, info);
    }

    emit finished();
}

void SAKBluetoothDeviceInfoComboBox::onDeviceDiscovered(
    const QBluetoothDeviceInfo &info)
{
    if (!isEnabled()) {
        return;
    }

    addItem(info.name(), QVariant::fromValue(info));
}

void SAKBluetoothDeviceInfoComboBox::onErrorOccurred(const QString &errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}

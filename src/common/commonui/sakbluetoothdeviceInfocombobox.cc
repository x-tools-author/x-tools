/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakbluetoothdeviceInfocombobox.h"

#include <QBluetoothDeviceInfo>
#include <QMessageBox>

SAKBluetoothDeviceInfoComboBox::SAKBluetoothDeviceInfoComboBox(QWidget* parent)
    : SAKComboBox(parent)
    , mScanner(Q_NULLPTR)
{
#if 0
    mScanner = new SAKBleScanner(this);
    connect(mScanner, &SAKBleScanner::finished,
            this, &SAKBluetoothDeviceInfoComboBox::onFinished);
    connect(mScanner, &SAKBleScanner::started,
            this, &SAKBluetoothDeviceInfoComboBox::started);
    connect(mScanner, &SAKBleScanner::deviceDiscovered,
            this, &SAKBluetoothDeviceInfoComboBox::onDeviceDiscovered);
    connect(mScanner, &SAKBleScanner::errorOccurred,
            this, &SAKBluetoothDeviceInfoComboBox::onErrorOccurred);
#endif
}

SAKBluetoothDeviceInfoComboBox::~SAKBluetoothDeviceInfoComboBox()
{
#if 0
    mScanner->stopDiscover();
#endif
}

void SAKBluetoothDeviceInfoComboBox::startDiscover()
{
#if 0
    clear();
    mScanner->startDiscover();
#endif
}

void SAKBluetoothDeviceInfoComboBox::stopDiscover()
{
#if 0
    mScanner->stopDiscover();
#endif
}

bool SAKBluetoothDeviceInfoComboBox::isActive()
{
#if 0
    return mScanner->isActive();
#endif
    return false;
}

void SAKBluetoothDeviceInfoComboBox::setTimeoutInterval(int interval)
{
#if 0
    mScanner->setTimeoutInterval(interval);
#endif
}

void SAKBluetoothDeviceInfoComboBox::setNameFiltter(const QString& filtter)
{
    //    mScanner->setNameFiltter(filtter);
}

void SAKBluetoothDeviceInfoComboBox::changeEvent(QEvent* event)
{
    SAKComboBox::changeEvent(event);
    if ((event->type() == QEvent::EnabledChange) && isEnabled()) {
        onFinished();
    }
}

void SAKBluetoothDeviceInfoComboBox::onFinished()
{
    // if (!isEnabled()) {
    //     return;
    // }

    // clear();
    // auto infos = mScanner->devicesInfoList();
    // for (auto &info : infos) {
    //     QString name = mScanner->deviceName(info);
    //     addItem(name, info);
    // }

    // emit finished();
}

void SAKBluetoothDeviceInfoComboBox::onDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    if (!isEnabled()) {
        return;
    }

    addItem(info.name(), QVariant::fromValue(info));
}

void SAKBluetoothDeviceInfoComboBox::onErrorOccurred(const QString& errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}

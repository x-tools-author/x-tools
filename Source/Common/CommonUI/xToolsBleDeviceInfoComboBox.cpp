/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBluetoothDeviceInfoComboBox.h"

#include <QBluetoothDeviceInfo>
#include <QMessageBox>

xToolsBluetoothDeviceInfoComboBox::xToolsBluetoothDeviceInfoComboBox(QWidget* parent)
    : xToolsComboBox(parent)
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

xToolsBluetoothDeviceInfoComboBox::~xToolsBluetoothDeviceInfoComboBox()
{
#if 0
    mScanner->stopDiscover();
#endif
}

void xToolsBluetoothDeviceInfoComboBox::startDiscover()
{
#if 0
    clear();
    mScanner->startDiscover();
#endif
}

void xToolsBluetoothDeviceInfoComboBox::stopDiscover()
{
#if 0
    mScanner->stopDiscover();
#endif
}

bool xToolsBluetoothDeviceInfoComboBox::isActive()
{
#if 0
    return mScanner->isActive();
#endif
    return false;
}

void xToolsBluetoothDeviceInfoComboBox::setTimeoutInterval(int interval)
{
#if 0
    mScanner->setTimeoutInterval(interval);
#endif
}

void xToolsBluetoothDeviceInfoComboBox::setNameFiltter(const QString& filtter)
{
    //    mScanner->setNameFiltter(filtter);
}

void xToolsBluetoothDeviceInfoComboBox::changeEvent(QEvent* event)
{
    xToolsComboBox::changeEvent(event);
    if ((event->type() == QEvent::EnabledChange) && isEnabled()) {
        onFinished();
    }
}

void xToolsBluetoothDeviceInfoComboBox::onFinished()
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

void xToolsBluetoothDeviceInfoComboBox::onDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    if (!isEnabled()) {
        return;
    }

    addItem(info.name(), QVariant::fromValue(info));
}

void xToolsBluetoothDeviceInfoComboBox::onErrorOccurred(const QString& errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}

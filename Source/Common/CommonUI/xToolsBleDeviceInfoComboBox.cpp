/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBleDeviceInfoComboBox.h"

#include <QBluetoothDeviceInfo>
#include <QMessageBox>

xToolsBleDeviceInfoComboBox::xToolsBleDeviceInfoComboBox(QWidget* parent)
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

xToolsBleDeviceInfoComboBox::~xToolsBleDeviceInfoComboBox()
{
#if 0
    mScanner->stopDiscover();
#endif
}

void xToolsBleDeviceInfoComboBox::startDiscover()
{
#if 0
    clear();
    mScanner->startDiscover();
#endif
}

void xToolsBleDeviceInfoComboBox::stopDiscover()
{
#if 0
    mScanner->stopDiscover();
#endif
}

bool xToolsBleDeviceInfoComboBox::isActive()
{
#if 0
    return mScanner->isActive();
#endif
    return false;
}

void xToolsBleDeviceInfoComboBox::setTimeoutInterval(int interval)
{
#if 0
    mScanner->setTimeoutInterval(interval);
#endif
}

void xToolsBleDeviceInfoComboBox::setNameFiltter(const QString& filtter)
{
    //    mScanner->setNameFiltter(filtter);
}

void xToolsBleDeviceInfoComboBox::changeEvent(QEvent* event)
{
    xToolsComboBox::changeEvent(event);
    if ((event->type() == QEvent::EnabledChange) && isEnabled()) {
        onFinished();
    }
}

void xToolsBleDeviceInfoComboBox::onFinished()
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

void xToolsBleDeviceInfoComboBox::onDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    if (!isEnabled()) {
        return;
    }

    addItem(info.name(), QVariant::fromValue(info));
}

void xToolsBleDeviceInfoComboBox::onErrorOccurred(const QString& errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}

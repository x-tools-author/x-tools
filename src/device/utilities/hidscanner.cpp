/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hidscanner.h"

#include <hidapi.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

#define MAX_STR 255

HidScanner::HidScanner(QObject *parent)
    : QThread{parent}
{
    static bool registered = false;
    if (!registered) {
        qRegisterMetaType<HidDeviceInfo>("HidDeviceInfo");
        registered = true;
    }
}

HidScanner::~HidScanner()
{
    exit();
    wait();
}

void HidScanner::setIsBusyDevicesIgnored(bool ignored)
{
    m_isBusyDevicesIgnored.store(ignored);
}

QList<HidDeviceInfo> HidScanner::hidInfos()
{
    return refresh();
}

void HidScanner::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [this, timer]() {
        refresh();
        timer->start();
    });

    refresh();
    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}

QList<HidDeviceInfo> HidScanner::refresh()
{
    hid_device_info *dev = hid_enumerate(0x0, 0x0);
    if (!dev) {
        return QList<HidDeviceInfo>();
    }

    QList<HidDeviceInfo> infos;
    while (dev) {
        HidDeviceInfo info;

        info.path = QString::fromUtf8(dev->path);
        info.vendorId = dev->vendor_id;
        info.productId = dev->product_id;
        info.serialNumber = QString::fromWCharArray(dev->serial_number);
        info.releaseNumber = dev->release_number;
        info.manufacturerString = QString::fromWCharArray(dev->manufacturer_string);
        info.productString = QString::fromWCharArray(dev->product_string);
        info.usagePage = dev->usage_page;
        info.usage = dev->usage;
        info.interfaceNumber = dev->interface_number;
        info.busType = static_cast<int>(dev->bus_type);

        infos.append(info);
        dev = dev->next;
    }

    hid_free_enumeration(dev);
    bool isEqual = std::equal(m_lastInfos.begin(),
                              m_lastInfos.end(),
                              infos.begin(),
                              infos.end(),
                              [](const HidDeviceInfo &a, const HidDeviceInfo &b) {
                                  bool isEqual = (a.path == b.path);
                                  isEqual &= (a.vendorId == b.vendorId);
                                  isEqual &= (a.productId == b.productId);
                                  isEqual &= (a.serialNumber == b.serialNumber);
                                  isEqual &= (a.releaseNumber == b.releaseNumber);
                                  isEqual &= (a.manufacturerString == b.manufacturerString);
                                  isEqual &= (a.productString == b.productString);
                                  isEqual &= (a.usagePage == b.usagePage);
                                  isEqual &= (a.usage == b.usage);
                                  isEqual &= (a.interfaceNumber == b.interfaceNumber);
                                  isEqual &= (a.busType == b.busType);
                                  return isEqual;
                              });
    if (!isEqual) {
        m_lastInfos = infos;
        emit devicesChanged(infos);
    }

    return infos;
}

bool HidScanner::isBusyDevice(const QString &portName)
{
    return false;
}

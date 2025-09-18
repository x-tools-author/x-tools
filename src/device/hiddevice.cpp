/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hiddevice.h"

#include <hidapi.h>

#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <QtMath>

#include "common/xtools.h"
#include "utilities/hidscanner.h"

HidDevice::HidDevice(QObject *parent)
    : Device(parent)
{}

HidDevice::~HidDevice() {}

QObject *HidDevice::initDevice()
{
    QVariantMap tmp = save();
    HidDeviceParameterKeys keys;
    QString path = tmp.value(keys.path).toString();
    if (path.isEmpty()) {
        emit errorOccurred(tr("HID device path is empty."));
        return nullptr;
    }

    m_hidDevice = hid_open_path(path.toUtf8().constData());
    if (!m_hidDevice) {
        emit errorOccurred(tr("Failed to open HID device \"%1\". ").arg(path));
        return nullptr;
    }

    hid_set_nonblocking(m_hidDevice, 1);
    m_readTimer = new QTimer();
    m_readTimer->setInterval(50);
    connect(m_readTimer, &QTimer::timeout, m_readTimer, [this]() { readBytesFromDevice(); });
    m_readTimer->start();
    return m_readTimer;
}

void HidDevice::deinitDevice()
{
    if (m_readTimer) {
        m_readTimer->stop();
        m_readTimer->deleteLater();
        m_readTimer = nullptr;
    }

    if (m_hidDevice) {
        hid_close(m_hidDevice);
        m_hidDevice = nullptr;
    }
}

void HidDevice::writeActually(const QByteArray &bytes)
{
    if (m_hidDevice) {
        const unsigned char *data = reinterpret_cast<const unsigned char *>(bytes.constData());
        int ret = hid_write(m_hidDevice, data, bytes.size());
        if (ret == -1) {
            QString errStr = QString::fromWCharArray(hid_error(m_hidDevice));
            emit errorOccurred(tr("Write HID device error: %1").arg(errStr));
        } else {
            emit bytesWritten(bytes, QString("HID"));
        }
    }
}

void HidDevice::readBytesFromDevice()
{
    if (!m_hidDevice) {
        return;
    }

    const int bufSize = 128;
    unsigned char buf[bufSize];
    int res = hid_read(m_hidDevice, buf, bufSize);
    if (res > 0) {
        QByteArray bytes = QByteArray::fromRawData(reinterpret_cast<const char *>(buf), res);
        emit bytesRead(bytes, QString("HID"));
    }
}

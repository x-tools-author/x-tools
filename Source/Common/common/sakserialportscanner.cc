/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakserialportscanner.h"
#include <QSerialPort>
#include <QSerialPortInfo>

SAKSerialPortScanner::SAKSerialPortScanner(QObject *parent)
    : QObject{parent}
{
    m_autoUpdatePortNamesTimer = new QTimer(this);
    m_autoUpdatePortNamesTimer->setInterval(1000);
    m_autoUpdatePortNamesTimer->setSingleShot(true);
    connect(m_autoUpdatePortNamesTimer, &QTimer::timeout, this, [=]() {
        refresh();
        m_autoUpdatePortNamesTimer->start();
    });

    refresh();
}

void SAKSerialPortScanner::refresh()
{
    auto temp = m_portNames;
    m_portNames.clear();
    auto infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        auto description = info.description();
        description = description.toUpper();
        if (description.contains("蓝牙") || description.contains("BLUETOOTH")) {
            continue;
        }

        if (m_ignoredBusyDevice) {
            if (!isBusy(info.portName())) {
                m_portNames.append(info.portName());
            }
        } else {
            m_portNames.append(info.portName());
        }
    }

    if (temp != m_portNames && !m_ignoredUpdate) {
        emit portNamesChanged();
    }

    temp = m_baudRates;
    m_baudRates.clear();
    auto bds = QSerialPortInfo::standardBaudRates();
    for (auto bd : bds) {
        m_baudRates.append(QString::number(bd));
    }

    if (temp != m_baudRates && !m_ignoredUpdate) {
        emit baudRatesChanged();
    }
}

void SAKSerialPortScanner::setIgnoredBusyDevice(bool ignored)
{
    m_ignoredBusyDevice = ignored;
    refresh();
}

void SAKSerialPortScanner::setAutoUpdatePortNames(bool autoUpdate)
{
    if (autoUpdate) {
        m_autoUpdatePortNamesTimer->start();
    } else {
        m_autoUpdatePortNamesTimer->stop();
    }
}

void SAKSerialPortScanner::setIgnoredUpdate(bool ignored)
{
    m_ignoredUpdate = ignored;
    if (!m_ignoredUpdate) {
        emit portNamesChanged();
    }
}

bool SAKSerialPortScanner::isBusy(const QString &portName)
{
    QSerialPort sp;
    sp.setPortName(portName);
    sp.setBaudRate(9600);
    bool busy = !sp.open(QSerialPort::ReadWrite);
    sp.close();
    return busy;
}

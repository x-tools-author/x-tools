/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialportscanner.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSerialPort>
#include <QTimer>

SerialPortScanner::SerialPortScanner(QObject *parent)
    : QThread{parent}
{
    m_isBusyDevicesIgnored.store(false);
    refresh();
}

SerialPortScanner::~SerialPortScanner()
{
    exit();
    wait();
}

void SerialPortScanner::setIsBusyDevicesIgnored(bool ignored)
{
    m_isBusyDevicesIgnored.store(ignored);
}

QStringList SerialPortScanner::baudRates() const
{
    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates();
    QStringList result;
    for (qint32 &rate : baudRates) {
        result.append(QString::number(rate));
    }

    return result;
}

QStringList SerialPortScanner::portNames()
{
    return refresh();
}

QList<QSerialPortInfo> SerialPortScanner::deviceInfos()
{
    return m_lastDevices;
}

void SerialPortScanner::run()
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

QStringList SerialPortScanner::refresh()
{
    QStringList portNames;
    auto infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        const QString portName = info.portName();
        if (portName.toLower().contains("bluetooth")) {
            continue;
        }

        if (m_isBusyDevicesIgnored.load()) {
            if (!isBusyDevice(portName)) {
                portNames.append(portName);
            }
        } else {
            portNames.append(info.portName());
        }
    }

#if 0
    bool isEqual = std::equal(portNames.begin(),
                              portNames.end(),
                              m_lastDevices.begin(),
                              m_lastDevices.end(),
                              [](const QString &name, const QSerialPortInfo &info) {
                                  return name == info.portName();
                              });
    if (!isEqual) {
        m_lastDevices = infos;
        emit devicesChanged(infos);
    }
#else
    if (portNames.size() != m_lastDevices.size()) {
        m_lastDevices = infos;
        emit devicesChanged(infos);
    } else {
        bool isEqual = true;
        for (int i = 0; i < portNames.size(); i++) {
            if (portNames.at(i) != m_lastDevices.at(i).portName()) {
                isEqual = false;
                break;
            }
        }

        if (!isEqual) {
            m_lastDevices = infos;
            emit devicesChanged(infos);
        }
    }
#endif

    emit portNamesChanged(portNames);
    return portNames;
}

bool SerialPortScanner::isBusyDevice(const QString &portName)
{
    QSerialPort sp;
    sp.setPortName(portName);
    sp.setBaudRate(9600);
    bool busy = !sp.open(QSerialPort::ReadWrite);
    sp.close();
    return busy;
}

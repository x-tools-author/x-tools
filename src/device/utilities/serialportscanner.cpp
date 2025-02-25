/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialportscanner.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

namespace xTools {

SerialPortScanner::SerialPortScanner(QObject *parent)
    : QThread{parent}
{
    refresh();
}

SerialPortScanner::~SerialPortScanner()
{
    exit();
    wait();
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

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}

void SerialPortScanner::refresh()
{
    m_mutex.lock();
    auto temp = m_portNames;
    m_mutex.unlock();

    QStringList newPortNames;
    auto infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        auto description = info.description();
        description = description.toUpper();

        if (m_isBusyDevicesIgnored.load()) {
            if (!isBusyDevice(info.portName())) {
                newPortNames.append(info.portName());
            }
        } else {
            newPortNames.append(info.portName());
        }
    }

    if (temp != newPortNames && !m_isBusy.load()) {
        m_mutex.lock();
        m_portNames = newPortNames;
        m_mutex.unlock();
        emit portNamesChanged();
    }

    m_mutex.lock();
    temp = m_baudRates;
    m_mutex.unlock();
    QStringList newBaudRates;
    auto bds = QSerialPortInfo::standardBaudRates();
    for (auto bd : bds) {
        newBaudRates.append(QString::number(bd));
    }

    if (temp != newBaudRates && !m_isBusy.load()) {
        m_mutex.lock();
        m_baudRates = newBaudRates;
        m_mutex.unlock();
        emit baudRatesChanged();
    }
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

QStringList SerialPortScanner::portNames()
{
    m_mutex.lock();
    auto tmp = m_portNames;
    m_mutex.unlock();

    return tmp;
}

QStringList SerialPortScanner::baudRates()
{
    m_mutex.lock();
    auto tmp = m_baudRates;
    m_mutex.unlock();

    return tmp;
}

bool SerialPortScanner::isBusyDevicesIgnored()
{
    return m_isBusyDevicesIgnored.load();
}

void SerialPortScanner::setIsBusyDevicesIgnored(bool ignored)
{
    m_isBusyDevicesIgnored.store(ignored);
    emit isBusyDevicesIgnoredChanged();
}

bool SerialPortScanner::isHotSwapEnabled()
{
    return m_hotSwapEnabled.load();
}
void SerialPortScanner::setIsHotSwapEnabled(bool enabled)
{
    m_hotSwapEnabled.store(enabled);
    emit isHotSwapEnabledChanged();
}

bool SerialPortScanner::isBusy()
{
    return m_isBusy.load();
}
void SerialPortScanner::setIsBusy(bool busy)
{
    m_isBusy.store(busy);
    emit isBusyChanged();
}

} // namespace xTools

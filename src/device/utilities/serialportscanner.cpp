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

SerialPortScanner::SerialPortScanner(QObject *parent)
    : QThread{parent}
{}

SerialPortScanner::~SerialPortScanner()
{
    exit();
    wait();
}

void SerialPortScanner::setIsBusyDevicesIgnored(bool ignored)
{
    m_isBusyDevicesIgnored.store(ignored);
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
    QStringList portNames;
    auto infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        if (m_isBusyDevicesIgnored.load()) {
            if (!isBusyDevice(info.portName())) {
                portNames.append(info.portName());
            }
        } else {
            portNames.append(info.portName());
        }
    }

    emit portNamesChanged(portNames);
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
/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QSerialPort>
#include <QSerialPortInfo>
#include "SAKSerialPortScanner.hh"

SAKSerialPortScanner::SAKSerialPortScanner(QObject *parent)
    : QObject{parent}
{
    nAutoUpdatePortNamesTimer = new QTimer(this);
    nAutoUpdatePortNamesTimer->setInterval(1000);
    nAutoUpdatePortNamesTimer->setSingleShot(true);
    connect(nAutoUpdatePortNamesTimer, &QTimer::timeout, this, [=](){
        refresh();
        nAutoUpdatePortNamesTimer->start();
    });

    refresh();
}

void SAKSerialPortScanner::refresh()
{
    auto temp = mPortNames;
    mPortNames.clear();
    auto infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        auto description = info.description();
        description = description.toUpper();
        if (description.contains("蓝牙") || description.contains("BLUETOOTH")) {
            continue;
        }

        if (mIgnoredBusyDevice) {
            if (!isBusy(info.portName())) {
                mPortNames.append(info.portName());
            }
        } else {
            mPortNames.append(info.portName());
        }
    }

    if (temp != mPortNames && !mIgnoredUpdate) {
        emit portNamesChanged();
    }

    temp = mBaudRates;
    mBaudRates.clear();
    auto bds = QSerialPortInfo::standardBaudRates();
    for (auto bd : bds) {
        mBaudRates.append(QString::number(bd));
    }

    if (temp != mBaudRates && !mIgnoredUpdate) {
        emit baudRatesChanged();
    }
}

void SAKSerialPortScanner::setIgnoredBusyDevice(bool ignored)
{
    mIgnoredBusyDevice = ignored;
    refresh();
}

void SAKSerialPortScanner::setAutoUpdatePortNames(bool autoUpdate)
{
    if (autoUpdate) {
        nAutoUpdatePortNamesTimer->start();
    } else {
        nAutoUpdatePortNamesTimer->stop();
    }
}

void SAKSerialPortScanner::setIgnoredUpdate(bool ignored)
{
    mIgnoredUpdate = ignored;
    if (!mIgnoredUpdate) {
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


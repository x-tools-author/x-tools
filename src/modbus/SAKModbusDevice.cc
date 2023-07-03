/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QSerialPort>
#include "SAKModbusDevice.hh"

SAKModbusDevice::SAKModbusDevice(const char *lc, QObject *parent)
    : QThread{parent}
    , mLoggingCategory(new QLoggingCategory(lc))
{
    mConnectParameters.serialport.baudRate = QSerialPort::Baud9600;
    mConnectParameters.serialport.dataBits = QSerialPort::Data8;
    mConnectParameters.serialport.stopBits = QSerialPort::OneStop;
    mConnectParameters.serialport.parity = QSerialPort::NoParity;

    mConnectParameters.network.ip = QString("127.0.0.1");
    mConnectParameters.network.port = 502;
}

SAKModbusDevice::~SAKModbusDevice()
{

}

void SAKModbusDevice::outputLog(const QString &log, int type)
{
    if (type == QtDebugMsg) {
        qCDebug((*mLoggingCategory)) << log;
    } else if (type == QtInfoMsg) {
        qCInfo((*mLoggingCategory)) << log;
    } else if (type == QtWarningMsg) {
        qCWarning((*mLoggingCategory)) << log;
    } else {
        qCCritical((*mLoggingCategory)) << log;
    }
}

QString SAKModbusDevice::portName()
{
    mConnectParametersMutex.lock();
    QString name = mConnectParameters.serialport.portName;
    mConnectParametersMutex.unlock();

    return name;
}

void SAKModbusDevice::setPortName(QString name)
{
    mConnectParametersMutex.lock();
    mConnectParameters.serialport.portName = name;
    mConnectParametersMutex.unlock();
}

int SAKModbusDevice::parity()
{
    mConnectParametersMutex.lock();
    int parity = mConnectParameters.serialport.parity;
    mConnectParametersMutex.unlock();

    return parity;
}

void SAKModbusDevice::setParity(int parity)
{
    mConnectParametersMutex.lock();
    mConnectParameters.serialport.parity = parity;
    mConnectParametersMutex.unlock();
}

int SAKModbusDevice::baudRate()
{
    mConnectParametersMutex.lock();
    int baudRate = mConnectParameters.serialport.baudRate;
    mConnectParametersMutex.unlock();

    return baudRate;
}

void SAKModbusDevice::setBaudRate(int baudRate)
{
    mConnectParametersMutex.lock();
    mConnectParameters.serialport.baudRate = baudRate;
    mConnectParametersMutex.unlock();
}

int SAKModbusDevice::dataBits()
{
    mConnectParametersMutex.lock();
    int dataBits = mConnectParameters.serialport.parity;
    mConnectParametersMutex.unlock();

    return dataBits;
}

void SAKModbusDevice::setDataBits(int bits)
{
    mConnectParametersMutex.lock();
    mConnectParameters.serialport.dataBits = bits;
    mConnectParametersMutex.unlock();
}

int SAKModbusDevice::stopBits()
{
    mConnectParametersMutex.lock();
    int stopBits = mConnectParameters.serialport.stopBits;
    mConnectParametersMutex.unlock();

    return stopBits;
}

void SAKModbusDevice::setStopBits(int bits)
{
    mConnectParametersMutex.lock();
    mConnectParameters.serialport.stopBits = bits;
    mConnectParametersMutex.unlock();
}

QString SAKModbusDevice::ip()
{
    mConnectParametersMutex.lock();
    QString ip = mConnectParameters.network.ip;
    mConnectParametersMutex.unlock();
    return ip;
}

void SAKModbusDevice::setIp(QString ip)
{
    mConnectParametersMutex.lock();
    mConnectParameters.network.ip = ip;
    mConnectParametersMutex.unlock();
}

int SAKModbusDevice::port()
{
    mConnectParametersMutex.lock();
    int port = mConnectParameters.network.port;
    mConnectParametersMutex.unlock();
    return port;
}

void SAKModbusDevice::setPort(int port)
{
    mConnectParametersMutex.lock();
    mConnectParameters.network.port = port;
    mConnectParametersMutex.unlock();
}

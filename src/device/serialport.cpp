/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialport.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <QtMath>

#include "common/xtools.h"

SerialPort::SerialPort(QObject *parent)
    : Device(parent)
{}

SerialPort::~SerialPort() {}

QObject *SerialPort::initDevice()
{
    QVariantMap tmp = save();
    SerialPortItem item = loadSerialPortItem(QJsonObject::fromVariantMap(tmp));
    m_serialPort = new QSerialPort();
    m_serialPort->setPortName(item.portName);
    m_serialPort->setBaudRate(item.baudRate);
    m_serialPort->setDataBits(static_cast<QSerialPort::DataBits>(item.dataBits));
    m_serialPort->setParity(static_cast<QSerialPort::Parity>(item.parity));
    m_serialPort->setStopBits(static_cast<QSerialPort::StopBits>(item.stopBits));
    m_serialPort->setFlowControl(static_cast<QSerialPort::FlowControl>(item.flowControl));

    qInfo() << "portName:" << m_serialPort->portName() << "baudRate:" << m_serialPort->baudRate()
            << "dataBits:" << m_serialPort->dataBits() << "parity:" << m_serialPort->parity()
            << "stopBits:" << m_serialPort->stopBits()
            << "flowControl:" << m_serialPort->flowControl()
            << "optimizedFrame:" << item.optimizedFrame;

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        bool optimizedFrame = item.optimizedFrame;
        connect(m_serialPort, &QSerialPort::readyRead, m_serialPort, [this, optimizedFrame]() {
            this->readBytesFromDevice(optimizedFrame);
        });
        connect(m_serialPort, &QSerialPort::errorOccurred, m_serialPort, [this]() {
            emit errorOccurred(m_serialPort->errorString());
        });
    } else {
        emit errorOccurred(tr("Failed to open serial port: %1").arg(m_serialPort->errorString()));
        m_serialPort->deleteLater();
        m_serialPort = nullptr;
    }

    return m_serialPort;
}

void SerialPort::deinitDevice()
{
    if (m_serialPort) {
        m_serialPort->close();
        m_serialPort->deleteLater();
        m_serialPort = nullptr;
    }
}

void SerialPort::writeActually(const QByteArray &bytes)
{
    if (m_serialPort) {
        qint64 ret = m_serialPort->write(bytes);
        if (ret == bytes.size()) {
            emit bytesWritten(bytes, m_serialPort->portName());
        }
    }
}

void SerialPort::readBytesFromDevice(bool optimizedFrame)
{
    if (!m_serialPort) {
        return;
    }

    if (optimizedFrame) {
        readBytesFromDeviceOptimized();
    } else {
        readBytesFromDeviceNormal();
    }
}

void SerialPort::readBytesFromDeviceNormal()
{
    QByteArray bytes = m_serialPort->readAll();
    if (!bytes.isEmpty()) {
        emit bytesRead(bytes, m_serialPort->portName());
    }
}

void SerialPort::readBytesFromDeviceOptimized()
{
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    QByteArray tmp;
    int delay = calculateInterFrameDelay();
    while (1) {
        QByteArray const data = m_serialPort->readAll();
        if (data.size() == 0) {
            if (elapsedTimer.elapsed() > delay) {
                if (!tmp.isEmpty()) {
                    emit bytesRead(tmp, m_serialPort->portName());
                }

                return;
            } else {
                continue;
            }
        }

        tmp.append(data);
        if (tmp.size() > 1024) {
            emit bytesRead(tmp, m_serialPort->portName());
            tmp.clear();
        }
    }
}

int SerialPort::calculateInterFrameDelay()
{
    // The spec recommends a timeout value of 1.750 msec. Without such
    // precise single-shot timers use a approximated value of 1.750 msec.
    int delayMilliSeconds = 2;
    qint32 baudRate = m_serialPort->baudRate();
    if (baudRate < 19200) {
        // Example: 9600 baud, 11 bit per packet -> 872 char/sec so:
        // 1000 ms / 872 char = 1.147 ms/char * 3.5 character = 4.0145 ms
        // Always round up because the spec requests at least 3.5 char.
        delayMilliSeconds = qCeil(3500. / (qreal(baudRate) / 11.));
    }

    return qMax(2, delayMilliSeconds);
}

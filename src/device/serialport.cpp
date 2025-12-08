/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialport.h"

#include <QDateTime>
#include <QDebug>
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
    SerialPortItem item = xLoadSerialPortItem(QJsonObject::fromVariantMap(tmp));
    m_serialPort = new QSerialPort();
    m_serialPort->setPortName(item.portName);
    m_serialPort->setBaudRate(item.baudRate);
    m_serialPort->setDataBits(static_cast<QSerialPort::DataBits>(item.dataBits));
    m_serialPort->setParity(static_cast<QSerialPort::Parity>(item.parity));
    m_serialPort->setStopBits(static_cast<QSerialPort::StopBits>(item.stopBits));
    m_serialPort->setFlowControl(static_cast<QSerialPort::FlowControl>(item.flowControl));
    m_interFrameDelayMilliseconds = item.interFrameDelayMilliseconds;

    qInfo() << "portName:" << m_serialPort->portName() << "baudRate:" << m_serialPort->baudRate()
            << "dataBits:" << m_serialPort->dataBits() << "parity:" << m_serialPort->parity()
            << "stopBits:" << m_serialPort->stopBits()
            << "flowControl:" << m_serialPort->flowControl()
            << "optimizedFrame:" << item.optimizedFrame
            << "interFrameDelayMilliseconds:" << m_interFrameDelayMilliseconds;

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit errorOccurred(tr("Failed to open serial port: %1").arg(m_serialPort->errorString()));
        m_serialPort->deleteLater();
        return nullptr;
    }

    bool optimizedFrame = item.optimizedFrame;
    connect(m_serialPort, &QSerialPort::readyRead, m_serialPort, [this, optimizedFrame]() {
        this->readBytesFromDevice(optimizedFrame);
    });
    connect(m_serialPort, &QSerialPort::errorOccurred, m_serialPort, [this]() {
        emit errorOccurred(m_serialPort->errorString());
    });

    m_checkTimer = new QTimer();
    m_checkTimer->setInterval(50);
    connect(m_checkTimer, &QTimer::timeout, m_checkTimer, [this]() { processCache(); });
    m_checkTimer->start();

    return m_serialPort;
}

void SerialPort::deinitDevice()
{
    if (m_checkTimer) {
        m_checkTimer->stop();
        m_checkTimer->deleteLater();
        m_checkTimer = nullptr;
    }

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

    QByteArray bytes = m_serialPort->readAll();
    if (bytes.isEmpty()) {
        return;
    }

    if (optimizedFrame) {
        m_cache.append(qMakePair(QDateTime::currentMSecsSinceEpoch(), bytes));
    } else {
        emit bytesRead(bytes, m_serialPort->portName());
    }
}

void SerialPort::processCache()
{
    if (m_cache.isEmpty()) {
        return;
    }

    QPair<qint64, QByteArray> lastItem = m_cache.last();
    qint64 endTime = lastItem.first;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime < (lastItem.first + m_interFrameDelayMilliseconds)) {
        // Waiting for more time...
        return;
    }

    while (!m_cache.isEmpty()) {
        QPair<qint64, QByteArray> firstItem = m_cache.first();
        qint64 toTime = firstItem.first + m_interFrameDelayMilliseconds;
        QByteArray frame;
        int n = 0;
        for (int i = 0; i < m_cache.size(); ++i) {
            QPair<qint64, QByteArray> item = m_cache.at(i);
            if (item.first <= toTime) {
                frame.append(item.second);
                n++;
            } else {
                break;
            }
        }

        for (int i = 0; i < n; ++i) {
            m_cache.removeFirst();
        }

        if (!frame.isEmpty()) {
            emit bytesRead(frame, m_serialPort->portName());
        }
    }
}
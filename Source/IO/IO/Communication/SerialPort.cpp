/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPort.h"

#include <QTimer>
#include <QtMath>

#include "../../xIO.h"

SerialPort::SerialPort(QObject *parent)
    : Communication(parent)
{}

SerialPort::~SerialPort() {}

QObject *SerialPort::initDevice()
{
    m_parametersMutex.lock();
    xIO::SerialPortItem item = xIO::loadSerialPortItem(QJsonObject::fromVariantMap(m_parameters));
    m_parametersMutex.unlock();

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
            << "flowControl:" << m_serialPort->flowControl();

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        connect(m_serialPort, &QSerialPort::readyRead, m_serialPort, [this]() {
            this->readBytesFromDevice();
        });
        connect(m_serialPort,
                &QSerialPort::errorOccurred,
                m_serialPort,
                [this](QSerialPort::SerialPortError error) {
                    emit errorOccurred(m_serialPort->errorString());
                });
    } else {
        emit errorOccurred(tr("Failed to open serial port: %1").arg(m_serialPort->errorString()));
        m_serialPort->deleteLater();
        m_serialPort = nullptr;
    }

    m_interFrameTimer = new QTimer();
    m_interFrameTimer->setInterval(m_interFrameDelayMilliseconds);
    m_interFrameTimer->setSingleShot(true);
    connect(m_interFrameTimer, &QTimer::timeout, m_interFrameTimer, [this]() {
        if (!this->m_frameBuffer.isEmpty()) {
            emit bytesRead(this->m_frameBuffer, m_serialPort->portName());
            this->m_frameBuffer.clear();
        }
    });
    return m_serialPort;
}

void SerialPort::deinitDevice()
{
    if (m_serialPort) {
        delete m_interFrameTimer;
        m_interFrameTimer = nullptr;

        m_serialPort->close();
        m_serialPort->deleteLater();
        m_serialPort = nullptr;
    }
}

void SerialPort::writeBytes(const QByteArray &bytes)
{
    if (m_serialPort) {
        qint64 ret = m_serialPort->write(bytes);
        if (ret == bytes.size()) {
            emit bytesWritten(bytes, m_serialPort->portName());
        }
    }
}

void SerialPort::readBytesFromDevice()
{
    if (!m_serialPort) {
        return;
    }

#if 1
    QByteArray bytes = m_serialPort->readAll();
    if (!bytes.isEmpty()) {
        emit bytesRead(bytes, m_serialPort->portName());
    }
#else
#if 0
    m_interFrameTimer->start(Qt::PreciseTimer);
    qint64 const size = m_serialPort->size();
    m_frameBuffer += m_serialPort->read(size);
    if (m_frameBuffer.size() > 1024) {
        emit bytesRead(m_frameBuffer, m_serialPort->portName());
        m_frameBuffer.clear();
    }
#else
    m_interFrameTimerElapsed.start();
    while (1) {
        qint64 const size = m_serialPort->size();
        if (size == 0) {
            if (m_interFrameTimerElapsed.elapsed() > m_interFrameDelayMilliseconds) {
                if (!m_frameBuffer.isEmpty()) {
                    emit bytesRead(m_frameBuffer, m_serialPort->portName());
                    m_frameBuffer.clear();
                }

                break;
            }

            continue;
        }

        m_frameBuffer += m_serialPort->read(size);
        m_interFrameTimerElapsed.restart();
        if (m_frameBuffer.size() > 1024) {
            emit bytesRead(m_frameBuffer, m_serialPort->portName());
            m_frameBuffer.clear();
        }
    }
#endif
#endif
}

void SerialPort::calculateInterFrameDelay()
{
    // The spec recommends a timeout value of 1.750 msec. Without such
    // precise single-shot timers use a approximated value of 1.750 msec.
    int delayMilliSeconds = s_recommendedDelay;
    qint32 baudRate = m_serialPort->baudRate();
    if (baudRate < 19200) {
        // Example: 9600 baud, 11 bit per packet -> 872 char/sec so:
        // 1000 ms / 872 char = 1.147 ms/char * 3.5 character = 4.0145 ms
        // Always round up because the spec requests at least 3.5 char.
        delayMilliSeconds = qCeil(3500. / (qreal(baudRate) / 11.));
    }
    m_interFrameDelayMilliseconds = qMax(m_interFrameDelayMilliseconds, delayMilliSeconds);
}

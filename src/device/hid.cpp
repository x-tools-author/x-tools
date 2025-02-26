/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hid.h"

#include <QTimer>
#include <QtMath>

#include "devicepage/common/xio.h"

namespace xTools {

Hid::Hid(QObject *parent)
    : Communication(parent)
{}

Hid::~Hid() {}

QObject *Hid::initDevice()
{
    m_parametersMutex.lock();
    xIO::HidItem item = xIO::loadHidItem(QJsonObject::fromVariantMap(m_parameters));
    m_parametersMutex.unlock();

    m_Hid = new QHid();
    m_Hid->setPortName(item.portName);
    m_Hid->setBaudRate(item.baudRate);
    m_Hid->setDataBits(static_cast<QHid::DataBits>(item.dataBits));
    m_Hid->setParity(static_cast<QHid::Parity>(item.parity));
    m_Hid->setStopBits(static_cast<QHid::StopBits>(item.stopBits));
    m_Hid->setFlowControl(static_cast<QHid::FlowControl>(item.flowControl));

    qInfo() << "portName:" << m_Hid->portName() << "baudRate:" << m_Hid->baudRate()
            << "dataBits:" << m_Hid->dataBits() << "parity:" << m_Hid->parity()
            << "stopBits:" << m_Hid->stopBits() << "flowControl:" << m_Hid->flowControl();

    if (m_Hid->open(QIODevice::ReadWrite)) {
        connect(m_Hid, &QHid::readyRead, m_Hid, [this]() { this->readBytesFromDevice(); });
        connect(m_Hid, &QHid::errorOccurred, m_Hid, [this](QHid::HidError error) {
            emit errorOccurred(m_Hid->errorString());
        });
    } else {
        emit errorOccurred(tr("Failed to open serial port: %1").arg(m_Hid->errorString()));
        m_Hid->deleteLater();
        m_Hid = nullptr;
    }

    m_interFrameTimer = new QTimer();
    m_interFrameTimer->setInterval(m_interFrameDelayMilliseconds);
    m_interFrameTimer->setSingleShot(true);
    connect(m_interFrameTimer, &QTimer::timeout, m_interFrameTimer, [this]() {
        if (!this->m_frameBuffer.isEmpty()) {
            emit bytesRead(this->m_frameBuffer, m_Hid->portName());
            this->m_frameBuffer.clear();
        }
    });
    return m_Hid;
}

void Hid::deinitDevice()
{
    if (m_Hid) {
        delete m_interFrameTimer;
        m_interFrameTimer = nullptr;

        m_Hid->close();
        m_Hid->deleteLater();
        m_Hid = nullptr;
    }
}

void Hid::writeBytes(const QByteArray &bytes)
{
    if (m_Hid) {
        qint64 ret = m_Hid->write(bytes);
        if (ret == bytes.size()) {
            emit bytesWritten(bytes, m_Hid->portName());
        }
    }
}

void Hid::readBytesFromDevice()
{
    if (!m_Hid) {
        return;
    }

#if 1
    QByteArray bytes = m_Hid->readAll();
    if (!bytes.isEmpty()) {
        emit bytesRead(bytes, m_Hid->portName());
    }
#else
#if 0
    m_interFrameTimer->start(Qt::PreciseTimer);
    qint64 const size = m_Hid->size();
    m_frameBuffer += m_Hid->read(size);
    if (m_frameBuffer.size() > 1024) {
        emit bytesRead(m_frameBuffer, m_Hid->portName());
        m_frameBuffer.clear();
    }
#else
    m_interFrameTimerElapsed.start();
    while (1) {
        qint64 const size = m_Hid->size();
        if (size == 0) {
            if (m_interFrameTimerElapsed.elapsed() > m_interFrameDelayMilliseconds) {
                if (!m_frameBuffer.isEmpty()) {
                    emit bytesRead(m_frameBuffer, m_Hid->portName());
                    m_frameBuffer.clear();
                }

                break;
            }

            continue;
        }

        m_frameBuffer += m_Hid->read(size);
        m_interFrameTimerElapsed.restart();
        if (m_frameBuffer.size() > 1024) {
            emit bytesRead(m_frameBuffer, m_Hid->portName());
            m_frameBuffer.clear();
        }
    }
#endif
#endif
}

void Hid::calculateInterFrameDelay()
{
    // The spec recommends a timeout value of 1.750 msec. Without such
    // precise single-shot timers use a approximated value of 1.750 msec.
    int delayMilliSeconds = s_recommendedDelay;
    qint32 baudRate = m_Hid->baudRate();
    if (baudRate < 19200) {
        // Example: 9600 baud, 11 bit per packet -> 872 char/sec so:
        // 1000 ms / 872 char = 1.147 ms/char * 3.5 character = 4.0145 ms
        // Always round up because the spec requests at least 3.5 char.
        delayMilliSeconds = qCeil(3500. / (qreal(baudRate) / 11.));
    }
    m_interFrameDelayMilliseconds = qMax(m_interFrameDelayMilliseconds, delayMilliSeconds);
}

} // namespace xTools

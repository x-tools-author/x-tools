/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSerialPortTool.h"

#include <QDebug>

#include "xToolsCompatibility.h"

xToolsSerialPortTool::xToolsSerialPortTool(QObject *parent)
    : xToolsCommunicationTool(parent)
{}

QString xToolsSerialPortTool::portName()
{
    return m_parameters.portName;
}

void xToolsSerialPortTool::setPortName(const QString &portName)
{
    m_parameters.portName = portName;
}

int xToolsSerialPortTool::baudRate()
{
    return m_parameters.baudRate;
}

void xToolsSerialPortTool::setBaudRate(int baudRate)
{
    m_parameters.baudRate = baudRate;
}

int xToolsSerialPortTool::dataBits()
{
    return m_parameters.dataBits;
}

void xToolsSerialPortTool::setDataBits(int dataBits)
{
    m_parameters.dataBits = dataBits;
}

int xToolsSerialPortTool::stopBits()
{
    return m_parameters.stopBits;
}

void xToolsSerialPortTool::setStopBits(int stopBits)
{
    m_parameters.stopBits = stopBits;
}

int xToolsSerialPortTool::parity()
{
    return m_parameters.parity;
}

void xToolsSerialPortTool::setParity(int parity)
{
    m_parameters.parity = parity;
}

int xToolsSerialPortTool::flowControl()
{
    return m_parameters.flowControl;
}

void xToolsSerialPortTool::setFlowControl(int flowControl)
{
    m_parameters.flowControl = flowControl;
}

bool xToolsSerialPortTool::initialize(QString &errStr)
{
    m_serialPort = new QSerialPort();
    m_serialPort->setPortName(m_parameters.portName);
    m_serialPort->setBaudRate(m_parameters.baudRate);
    m_serialPort->setDataBits(QSerialPort::DataBits(m_parameters.dataBits));
    m_serialPort->setStopBits(QSerialPort::StopBits(m_parameters.stopBits));
    m_serialPort->setParity(QSerialPort::Parity(m_parameters.parity));
    auto cookedFlowControl = QSerialPort::FlowControl(m_parameters.flowControl);
    m_serialPort->setFlowControl(cookedFlowControl);

    QString info = QString("portName:%1, baudRate:%2, dataBits:%3, "
                           "stopBits:%4, parity:%5, flowControl: %6")
                       .arg(m_parameters.portName,
                            QString::number(m_parameters.baudRate),
                            QString::number(m_parameters.dataBits),
                            QString::number(m_parameters.stopBits),
                            QString::number(m_parameters.parity),
                            QString::number(m_parameters.flowControl));
    qInfo() << "Open serialport with parameters:[" << qPrintable(info) << "]";

    if (!m_serialPort->open(QSerialPort::ReadWrite)) {
        errStr = "open serial port failed:";
        qWarning() << errStr << qPrintable(m_serialPort->errorString());
        m_serialPort->deleteLater();
        m_serialPort = Q_NULLPTR;
        return false;
    }

    connect(m_serialPort, &QSerialPort::readyRead, m_serialPort, [=]() { readBytes(); });

    return true;
}

void xToolsSerialPortTool::writeBytes(const QByteArray &bytes)
{
    if (m_serialPort && m_serialPort->isOpen()) {
        qint64 ret = m_serialPort->write(bytes);
        if (ret == -1) {
            qWarning() << m_serialPort->errorString();
        } else if (ret > 0) {
            QByteArray hex = xToolsByteArrayToHex(bytes, ' ');
            QString msg = QString::fromLatin1(hex);
            msg = QString("%1<-%2").arg(m_parameters.portName, msg);
            qInfo() << msg;
            emit bytesWritten(bytes, m_serialPort->portName());
        }
    }
}

void xToolsSerialPortTool::uninitialize()
{
    if (m_serialPort) {
        m_serialPort->close();
        m_serialPort->deleteLater();
        m_serialPort = Q_NULLPTR;
    }
}

void xToolsSerialPortTool::readBytes()
{
    if (m_serialPort && m_serialPort->isOpen()) {
        QByteArray bytes = m_serialPort->readAll();
        if (bytes.isEmpty()) {
            return;
        }
#if 1
        QByteArray hex = xToolsByteArrayToHex(bytes, ' ');
        QString msg = QString::fromLatin1(hex);
        msg = QString("%1<-%2").arg(m_parameters.portName, msg);
        qInfo() << qPrintable(msg);
#endif
        emit outputBytes(bytes);
        emit bytesRead(bytes, m_serialPort->portName());
    }
}

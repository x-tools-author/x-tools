/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakserialporttool.h"
#include "sakinterface.h"

SAKSerialPortTool::SAKSerialPortTool(QObject *parent)
    : SAKCommunicationTool("sak.serialporttool", parent)
{}

QString SAKSerialPortTool::portName()
{
    return mParameters.portName;
}

void SAKSerialPortTool::setPortName(const QString &portName)
{
    mParameters.portName = portName;
}

int SAKSerialPortTool::baudRate()
{
    return mParameters.baudRate;
}

void SAKSerialPortTool::setBaudRate(int baudRate)
{
    mParameters.baudRate = baudRate;
}

int SAKSerialPortTool::dataBits()
{
    return mParameters.dataBits;
}

void SAKSerialPortTool::setDataBits(int dataBits)
{
    mParameters.dataBits = dataBits;
}

int SAKSerialPortTool::stopBits()
{
    return mParameters.stopBits;
}

void SAKSerialPortTool::setStopBits(int stopBits)
{
    mParameters.stopBits = stopBits;
}

int SAKSerialPortTool::parity()
{
    return mParameters.parity;
}

void SAKSerialPortTool::setParity(int parity)
{
    mParameters.parity = parity;
}

int SAKSerialPortTool::flowControl()
{
    return mParameters.flowControl;
}

void SAKSerialPortTool::setFlowControl(int flowControl)
{
    mParameters.flowControl = flowControl;
}

bool SAKSerialPortTool::initialize(QString &errStr)
{
    mSerialPort = new QSerialPort();
    mSerialPort->setPortName(mParameters.portName);
    mSerialPort->setBaudRate(mParameters.baudRate);
    mSerialPort->setDataBits(QSerialPort::DataBits(mParameters.dataBits));
    mSerialPort->setStopBits(QSerialPort::StopBits(mParameters.stopBits));
    mSerialPort->setParity(QSerialPort::Parity(mParameters.parity));
    auto cookedFlowControl = QSerialPort::FlowControl(mParameters.flowControl);
    mSerialPort->setFlowControl(cookedFlowControl);

    mToolName = mParameters.portName;

    QString info = QString("portName:%1, baudRate:%2, dataBits:%3, "
                           "stopBits:%4, parity:%5, flowControl: %6")
                       .arg(mParameters.portName,
                            QString::number(mParameters.baudRate),
                            QString::number(mParameters.dataBits),
                            QString::number(mParameters.stopBits),
                            QString::number(mParameters.parity),
                            QString::number(mParameters.flowControl));
    outputMessage(QtInfoMsg, info);

    if (!mSerialPort->open(QSerialPort::ReadWrite)) {
        errStr = "open serial port failed:" + mSerialPort->errorString();
        outputMessage(QtWarningMsg, errStr);
        mSerialPort->deleteLater();
        mSerialPort = Q_NULLPTR;
        return false;
    }

    connect(mSerialPort, &QSerialPort::readyRead, mSerialPort, [=]() { readBytes(); });

    return true;
}

void SAKSerialPortTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    if (mSerialPort && mSerialPort->isOpen()) {
        qint64 ret = mSerialPort->write(bytes);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mSerialPort->errorString());
        } else if (ret > 0) {
            QByteArray hex = SAKInterface::arrayToHex(bytes, ' ');
            QString msg = QString::fromLatin1(hex);
            msg = QString("%1<-%2").arg(mParameters.portName, msg);
            outputMessage(QtInfoMsg, msg);
            emit bytesInputted(bytes, context);
        }
    }
}

void SAKSerialPortTool::readBytes()
{
    if (mSerialPort && mSerialPort->isOpen()) {
        QByteArray bytes = mSerialPort->readAll();
        if (!bytes.isEmpty()) {
            QByteArray hex = SAKInterface::arrayToHex(bytes, ' ');
            QString msg = QString::fromLatin1(hex);
            msg = QString("%1<-%2").arg(mParameters.portName, msg);
            outputMessage(QtInfoMsg, msg);
            emit bytesOutputted(bytes, rxJsonObject());
        }
    }
}

void SAKSerialPortTool::uninitialize()
{
    if (mSerialPort) {
        mSerialPort->close();
        mSerialPort->deleteLater();
        mSerialPort = Q_NULLPTR;
    }
}

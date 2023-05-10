/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSerialPortTool.hh"

SAKSerialPortTool::SAKSerialPortTool(QObject *parent)
    : SAKCommunicationTool("SAK.SerialPortTool", parent)
{

}

void SAKSerialPortTool::setPortName(const QString &portName)
{
    mParameters.portName = portName;
}

void SAKSerialPortTool::setBaudRate(int baudRate)
{
    mParameters.baudRate = baudRate;
}

void SAKSerialPortTool::setDataBits(int dataBits)
{
    mParameters.dataBits = dataBits;
}

void SAKSerialPortTool::setStopBits(int stopBits)
{
    mParameters.stopBits = stopBits;
}

void SAKSerialPortTool::setParity(int parity)
{
    mParameters.parity = parity;
}

void SAKSerialPortTool::setFlowControl(int flowControl)
{
    mParameters.flowControl = flowControl;
}

bool SAKSerialPortTool::initialize()
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
        outputMessage(QtWarningMsg, mSerialPort->errorString());
        mSerialPort->deleteLater();
        mSerialPort = Q_NULLPTR;
        return false;
    }

    mIsTimerReading = false;
    connect(mSerialPort, &QSerialPort::readyRead, mSerialPort, [=](){
        readBytes();
    });

    return true;
}

void SAKSerialPortTool::writeBytes(const QByteArray &bytes, const QVariant &context)
{
    if (mSerialPort && mSerialPort->isOpen()) {
        qint64 ret = mSerialPort->write(bytes);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mSerialPort->errorString());
        } else if (ret > 0) {
            outputMessage(QtInfoMsg,
                          qPrintable("Tx:" + QString(bytes.toHex(' '))));
            emit bytesInputted(bytes, context);
        }
    }
}

void SAKSerialPortTool::readBytes()
{
    if (mSerialPort && mSerialPort->isOpen()) {
        QByteArray bytes = mSerialPort->readAll();
        if (!bytes.isEmpty()) {
            outputMessage(QtInfoMsg,
                          qPrintable("Rx:" + QString(bytes.toHex(' '))));
            emit bytesOutputted(bytes, QVariant());
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

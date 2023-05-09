/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDSerialPortTool.hpp"

EDSerialPortTool::EDSerialPortTool(QObject *parent)
    : EDBaseTool("ED.SerialPortTool", parent)
{

}

void EDSerialPortTool::setPortName(const QString &portName)
{
    mParameters.portName = portName;
}

void EDSerialPortTool::setBaudRate(int baudRate)
{
    mParameters.baudRate = baudRate;
}

void EDSerialPortTool::setDataBits(int dataBits)
{
    mParameters.dataBits = dataBits;
}

void EDSerialPortTool::setStopBits(int stopBits)
{
    mParameters.stopBits = stopBits;
}

void EDSerialPortTool::setParity(int parity)
{
    mParameters.parity = parity;
}

void EDSerialPortTool::setFlowControl(int flowControl)
{
    mParameters.flowControl = flowControl;
}

bool EDSerialPortTool::initialize(QString &errStr)
{
    mSerialPort = new QSerialPort();
    mSerialPort->setPortName(mParameters.portName);
    mSerialPort->setBaudRate(mParameters.baudRate);
    mSerialPort->setDataBits(QSerialPort::DataBits(mParameters.dataBits));
    mSerialPort->setStopBits(QSerialPort::StopBits(mParameters.stopBits));
    mSerialPort->setParity(QSerialPort::Parity(mParameters.parity));
    auto cookedFlowControl = QSerialPort::FlowControl(mParameters.flowControl);
    mSerialPort->setFlowControl(cookedFlowControl);

    setToolFlag(mParameters.portName);

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

    connect(mSerialPort, &QSerialPort::readyRead, mSerialPort, [=](){
        emit invokeOutputBytes(EDPrivateSignal{});
    });

    return true;
}

void EDSerialPortTool::inputBytesHandler(const QByteArray &bytes)
{
    if (mSerialPort && mSerialPort->isOpen()) {
        qint64 ret = mSerialPort->write(bytes);
        if (ret == -1) {
            outputMessage(QtWarningMsg, mSerialPort->errorString());
        } else if (ret > 0) {
            outputMessage(QtInfoMsg,
                          qPrintable("Tx:" + QString(bytes.toHex(' '))));
            emit bytesInputted(toolFlag(), bytes);
        }
    }
}

void EDSerialPortTool::outputBytesHandler()
{
    if (mSerialPort && mSerialPort->isOpen()) {
        QByteArray bytes = mSerialPort->readAll();
        if (!bytes.isEmpty()) {
            outputMessage(QtInfoMsg,
                          qPrintable("Rx:" + QString(bytes.toHex(' '))));
            emit bytesOutputted(toolFlag(), bytes);
        }
    }
}

void EDSerialPortTool::uninitialize()
{
    if (mSerialPort) {
        mSerialPort->close();
        mSerialPort->deleteLater();
        mSerialPort = Q_NULLPTR;
    }
}

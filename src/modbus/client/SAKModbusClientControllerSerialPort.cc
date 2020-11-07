/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>

#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"

SAKModbusClientControllerSerialPort::SAKModbusClientControllerSerialPort(QWidget *parent)
    :SAKModbusClientController(parent)
{
    init();
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    appendSection(mSerialPortSection);
    mClient = qobject_cast<QModbusRtuSerialMaster*>(device());
    connect(mClient, &QModbusClient::stateChanged, this, [=](){
        bool isUnconnected = (mClient->state() == QModbusDevice::UnconnectedState);
        mSerialPortSection->setEnabled(isUnconnected);
        mClientSection->setUiEnable(isUnconnected);
    });
}

void SAKModbusClientControllerSerialPort::open()
{
    SAKModbusCommonSerialPortSection::ParametersContext seruialPortParasCtx = mSerialPortSection->parametersContext();
    mClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, seruialPortParasCtx.portName);
    mClient->setConnectionParameter(QModbusDevice::SerialParityParameter, seruialPortParasCtx.parity);
    mClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, seruialPortParasCtx.baudRate);
    mClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, seruialPortParasCtx.dataBits);
    mClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, seruialPortParasCtx.stopBits);
    SAKModbusCommonClientSection::ParametersContext clientParacCtx = mClientSection->parametersContext();
    mClient->setNumberOfRetries(clientParacCtx.numberOfRetries);
    mClient->setTimeout(clientParacCtx.timeout);
    // The function is asynchronous
    mClient->connectDevice();
}

QModbusDevice *SAKModbusClientControllerSerialPort::initModbusDevice()
{
    auto dev = new QModbusRtuSerialMaster;
    return dev;
}

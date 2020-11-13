/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonServerSection.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusServerControllerSerialPort.hh"

SAKModbusServerControllerSerialPort::SAKModbusServerControllerSerialPort(QWidget *parent)
    :SAKModbusServerController(parent)
{
    init();
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    appendSection(mSerialPortSection);
    mServer = qobject_cast<QModbusRtuSerialSlave*>(device());
    connect(mServer, &QModbusServer::stateChanged, [=]{
        bool isunConnecnted = mServer->state() == QModbusDevice::UnconnectedState;
        mSerialPortSection->setEnabled(isunConnecnted);
    });
}

void SAKModbusServerControllerSerialPort::open()
{
    mSerialPortSection->initModbusDeviceParamerers(mServer);
    mServerSection->initModbusServerParameters(mServer);
    mServer->connectDevice();
}

QModbusDevice *SAKModbusServerControllerSerialPort::initModbusDevice()
{
    auto dev = new QModbusRtuSerialSlave;
    return dev;
}

/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonHostSection.hh"
#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusClientControllerTcp.hh"

SAKModbusClientControllerTcp::SAKModbusClientControllerTcp(QWidget *parent)
    :SAKModbusClientController(parent)
{
    init();
    mHostSection = new SAKModbusCommonHostSection(this);
    appendSection(mHostSection);

    mClient = qobject_cast<QModbusTcpClient*>(device());
    connect(mClient, &QModbusTcpClient::stateChanged, this, [=](){
        bool isUnconnected = mClient->state() == QModbusDevice::UnconnectedState;
        mHostSection->setEnabled(isUnconnected);
        mClientSection->setUiEnable(isUnconnected);
    });
}

void SAKModbusClientControllerTcp::open()
{
    mHostSection->initModbusDeviceParamerers(mClient);
    mClientSection->initModbusClientParameters(mClient);
    mClient->connectDevice();
}

QModbusDevice *SAKModbusClientControllerTcp::initModbusDevice()
{
    auto dev = new QModbusTcpClient;
    return dev;
}

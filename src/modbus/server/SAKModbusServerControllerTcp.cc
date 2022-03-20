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
#include <QModbusTcpServer>
#include "SAKModbusCommonHostSection.hh"
#include "SAKModbusCommonServerSection.hh"
#include "SAKModbusServerControllerTcp.hh"

SAKModbusServerControllerTcp::SAKModbusServerControllerTcp(QWidget *parent)
    :SAKModbusServerController(parent)
{
    init();
    mHostSection = new SAKModbusCommonHostSection(this);
    appendSection(mHostSection);
    mServer = qobject_cast<QModbusTcpServer*>(device());
    connect(mServer, &QModbusServer::stateChanged, [=]{
        bool isunConnecnted = mServer->state() == QModbusDevice::UnconnectedState;
        mHostSection->setEnabled(isunConnecnted);
    });
}

void SAKModbusServerControllerTcp::open()
{
    mHostSection->initModbusDeviceParamerers(mServer);
    mServerSection->initModbusServerParameters(mServer);
    mServer->connectDevice();
}

QModbusDevice *SAKModbusServerControllerTcp::initModbusDevice()
{
    auto dev = new QModbusTcpServer;
    return dev;
}

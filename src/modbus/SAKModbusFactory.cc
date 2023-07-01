/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/

#include "SAKModbusFactory.hh"

SAKModbusFactory::SAKModbusFactory(QObject *parent)
    : QObject(parent)
{

}

SAKModbusFactory::~SAKModbusFactory()
{

}

const QString SAKModbusFactory::typeName(int type)
{
    if (type == ModbusRtuSerialClient) {
        return tr("RTU Client");
    } else if (type == ModbusRtuSerialServer) {
        return tr("RTU Server");
    } else if (type == ModbusTcpServer) {
        return tr("TCP Client");
    } else if (type == ModbusTcpClient) {
        return tr("TCP Server");
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown modebus device type");
    qCWarning(mLoggingCategory) << "Unknown modebus device type";

    return "Unknown";
}

/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QVariant>
#include <QModbusTcpClient>
#include "SAKModbusTcpClient.hh"

SAKModbusTcpClient::SAKModbusTcpClient(QObject *parent)
    : SAKModbusClient("sak.modbus.tcpclient", parent)
{

}

QModbusClient *SAKModbusTcpClient::createClient()
{
    QModbusTcpClient *client = new QModbusTcpClient();

    client->setConnectionParameter(QModbusClient::NetworkAddressParameter,
                                   QVariant::fromValue(ip()));
    client->setConnectionParameter(QModbusClient::NetworkPortParameter,
                                   QVariant::fromValue(port()));

    client->setNumberOfRetries(numberOfRetries());
    client->setTimeout(timeout());

    return client;
}

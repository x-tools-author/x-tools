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
#include <QModbusRtuSerialClient>
#include "SAKModbusRtuSerialClient.hh"

SAKModbusRtuSerialClient::SAKModbusRtuSerialClient(QObject *parent)
    : SAKModbusClient("sak.modbus.rtuserialclient", parent)
{

}

QModbusClient *SAKModbusRtuSerialClient::createClient()
{
    QModbusRtuSerialClient *client = new QModbusRtuSerialClient();

    client->setConnectionParameter(QModbusClient::SerialPortNameParameter,
                                   QVariant::fromValue(portName()));
    client->setConnectionParameter(QModbusClient::SerialBaudRateParameter,
                                   QVariant::fromValue(baudRate()));
    client->setConnectionParameter(QModbusClient::SerialDataBitsParameter,
                                   QVariant::fromValue(dataBits()));
    client->setConnectionParameter(QModbusClient::SerialStopBitsParameter,
                                   QVariant::fromValue(stopBits()));
    client->setConnectionParameter(QModbusClient::SerialParityParameter,
                                   QVariant::fromValue(parity()));

    client->setNumberOfRetries(numberOfRetries());
    client->setTimeout(timeout());

    return client;
}

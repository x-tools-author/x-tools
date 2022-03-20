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
#include "SAKModbusServerController.hh"
#include "SAKModbusCommonServerSection.hh"

SAKModbusServerController::SAKModbusServerController(QWidget *parent)
    :SAKModbusCommonController(parent)
{
    mServerSection = new SAKModbusCommonServerSection(this);
}

QWidget *SAKModbusServerController::bottomSection()
{
    return mServerSection;
}

void SAKModbusServerController::setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value)
{
    auto server = qobject_cast<QModbusServer*>(device());
    if (server){
        if ((type == QModbusDataUnit::Coils) || (type == QModbusDataUnit::DiscreteInputs)){
            server->setData(type, address, value ? true : false);
        }else {
            server->setData(type, address, value);
        }
    }
}

quint16 SAKModbusServerController::registerValue(QModbusDataUnit::RegisterType type, quint16 address)
{
    quint16 value = 0;
    auto server = qobject_cast<QModbusServer*>(device());
    if (server){
        if (!server->data(type, address, &value)){
            QString err = tr("Can not get the value of register which type is:%1").arg(type);
            emit invokeOutputMessage(err);
        }
    }

    return value;
}

void SAKModbusServerController::exportRegisterData()
{
    auto fileName = getSaveFileName();
    if (fileName.length()){
        saveServerRegisterData(qobject_cast<QModbusServer*>(device()), fileName);
    }
}

void SAKModbusServerController::importRegisterData()
{
    auto fileName = getOpenFileName();
    if (fileName.length()){
        setServerRegisterData(qobject_cast<QModbusServer*>(device()), fileName);
    }
}

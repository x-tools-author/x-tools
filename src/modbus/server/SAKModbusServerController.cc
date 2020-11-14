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

        qDebug() << __FUNCTION__ << type << address << value;
    }
}

quint16 SAKModbusServerController::registerValue(QModbusDataUnit::RegisterType type, quint16 address)
{
    quint16 value = 0;
    auto server = qobject_cast<QModbusServer*>(device());
    if (server){
        if (!server->data(type, address, &value)){
            qWarning() << "Can not get the value of register which type is:" << type;
        }
    }

    return value;
}

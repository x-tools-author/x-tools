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
    :SAKModbusCommonController(parent)
{
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    mServerSection = new SAKModbusCommonServerSection(this);
    appendSection(mSerialPortSection);
    appendSection(mServerSection);
}

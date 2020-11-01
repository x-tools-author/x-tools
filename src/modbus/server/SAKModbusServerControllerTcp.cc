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
#include "SAKModbusCommonServerSection.hh"
#include "SAKModbusServerControllerTcp.hh"

SAKModbusServerControllerTcp::SAKModbusServerControllerTcp(QWidget *parent)
    :SAKModbusCommonController(parent)
{
    mHostSection = new SAKModbusCommonHostSection(this);
    mServerSection = new SAKModbusCommonServerSection(this);
    appendSection(mHostSection);
    appendSection(mServerSection);
}

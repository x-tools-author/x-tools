/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSSERVERCONTROLLERTCP_HH
#define SAKMODBUSSERVERCONTROLLERTCP_HH

#include <QModbusRtuSerialSlave>
#include "SAKModbusServerController.hh"

class SAKModbusCommonHostSection;
class SAKModbusServerControllerTcp : public SAKModbusServerController
{
    Q_OBJECT
public:
    SAKModbusServerControllerTcp(QWidget *parent = Q_NULLPTR);
    virtual void open() final;
protected:
    virtual QModbusDevice *initModbusDevice() final;
private:
    SAKModbusCommonHostSection *mHostSection;
    QModbusRtuSerialSlave *mServer;
};

#endif // SAKMODBUSSERVERCONTROLLERTCP_HH

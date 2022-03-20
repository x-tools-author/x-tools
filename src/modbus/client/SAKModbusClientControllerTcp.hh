/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCLIENTCONTROLLERTCP_HH
#define SAKMODBUSCLIENTCONTROLLERTCP_HH

#include <QModbusTcpClient>
#include "SAKModbusClientController.hh"

class SAKModbusCommonHostSection;
class SAKModbusCommonClientSection;
class SAKModbusClientControllerTcp : public SAKModbusClientController
{
    Q_OBJECT
public:
    SAKModbusClientControllerTcp(QWidget *parent = Q_NULLPTR);

    virtual void open() final;
protected:
    virtual QModbusDevice *initModbusDevice() final;
private:
    SAKModbusCommonHostSection *mHostSection;
    QModbusTcpClient *mClient;
};

#endif // SAKMODBUSCLIENTCONTROLLERTCP_HH

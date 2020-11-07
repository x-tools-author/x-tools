/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH
#define SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH

#include <QModbusRtuSerialMaster>
#include "SAKModbusClientController.hh"

class SAKModbusCommonClientSection;
class SAKModbusCommonSerialPortSection;
class SAKModbusClientControllerSerialPort : public SAKModbusClientController
{
public:
    SAKModbusClientControllerSerialPort(QWidget *parent = Q_NULLPTR);
    virtual void open() final;
protected:
    QModbusDevice *initModbusDevice() final;
private:
    SAKModbusCommonSerialPortSection *mSerialPortSection;
    QModbusRtuSerialMaster *mClient;
};

#endif // SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH

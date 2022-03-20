/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCONTROLLERSERVERSERIALPORT_HH
#define SAKMODBUSCONTROLLERSERVERSERIALPORT_HH

#include "SAKModbusServerController.hh"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QModbusRtuSerialSlave>
#else
#include <QModbusRtuSerialServer>
#endif

class SAKModbusCommonSerialPortSection;
class SAKModbusServerControllerSerialPort : public SAKModbusServerController
{
    Q_OBJECT
public:
    SAKModbusServerControllerSerialPort(QWidget *parent = Q_NULLPTR);
    virtual void open() final;
protected:
    virtual QModbusDevice *initModbusDevice() final;
private:
    SAKModbusCommonSerialPortSection *mSerialPortSection;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QModbusRtuSerialSlave *mServer;
#else
    QModbusRtuSerialServer *mServer;
#endif
};

#endif // SAKMODBUSCONTROLLERSERVERSERIALPORT_HH

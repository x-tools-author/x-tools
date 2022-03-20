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

#include "SAKModbusClientController.hh"
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QModbusRtuSerialMaster>
#else
#include <QModbusRtuSerialClient>
#endif

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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QModbusRtuSerialMaster *mClient;
#else
    QModbusRtuSerialClient *mClient;
#endif
};

#endif // SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH

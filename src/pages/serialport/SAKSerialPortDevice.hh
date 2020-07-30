/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QMutex>
#include <QThread>
#include <QSerialPort>
#include <QWaitCondition>

#include "SAKDebugPageDevice.hh"

class SAKSerialPortDebugPage;
class SAKSerialPortDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKSerialPortDevice(SAKSerialPortDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKSerialPortDevice();
protected:
    void run() final;
private:
    QString                   name;
    qint32                    baudRate;
    QSerialPort::DataBits     dataBits;
    QSerialPort::StopBits     stopBits;
    QSerialPort::Parity       parity;
    QSerialPort::FlowControl  flowControl;
    QSerialPort               *serialPort;
    SAKSerialPortDebugPage    *debugPage;
};

#endif

/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QMutex>
#include <QThread>
#include <QSerialPort>
#include <QWaitCondition>

#include "SAKDevice.hh"

class SAKSerialPortDebugPage;
class SAKSerialPortDevice:public SAKDevice
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

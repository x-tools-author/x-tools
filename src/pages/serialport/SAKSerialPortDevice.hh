/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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

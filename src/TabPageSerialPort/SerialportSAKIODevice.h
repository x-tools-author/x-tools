/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SERIALPORTSAKIODEVICE_H
#define SERIALPORTSAKIODEVICE_H

#include <QSerialPort>
#include <QThread>
#include <QDebug>

#include "SAKIODevice.h"

class SerialportSAKIODevice : public SAKIODevice
{
    Q_OBJECT
public:
    SerialportSAKIODevice(QString deviceName = QString("SerialPort"), QObject *parent = Q_NULLPTR);
    ~SerialportSAKIODevice() override;
    /// -------------------------------------------------
    bool isOpen() override {return mpSerialPort->isOpen();}
    QString errorString() override {return mpSerialPort->errorString();}
public slots:
    void close() override;
    void open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity) override;
    void writeBytes(QByteArray data) override;

    void ReadAll();
protected:
    void run() override;
private:
    /// 串口
    QSerialPort *mpSerialPort = nullptr;
signals:
    /// 串口关闭成功
    void SerialPortCloseSuccessfully();
    /// 串口打开成功后发射该信号
    void SerialPortOpenSuccessfully();
    /// 外部程序关联该信号，可以获取需要输出的信息
    void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};

#endif

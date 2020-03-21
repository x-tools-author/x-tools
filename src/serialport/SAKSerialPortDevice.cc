/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QApplication>
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"

SAKSerialPortDevice::SAKSerialPortDevice(const QString name,
                                               const qint32 baudRate,
                                               const QSerialPort::DataBits dataBits,
                                               const QSerialPort::StopBits stopBits,
                                               const QSerialPort::Parity parity, SAKSerialPortDebugPage *debugPage,
                                               QObject *parent)
    :QThread (parent)
    ,_name (name)
    ,_baudRate (baudRate)
    ,_dataBits (dataBits)
    ,_stopBits (stopBits)
    ,_parity (parity)
    ,debugPage (debugPage)
{
    moveToThread(this);
}

SAKSerialPortDevice::~SAKSerialPortDevice()
{
//    if (serialPort){
//        serialPort->blockSignals(true);
//        serialPort->close();
//    }
}

void SAKSerialPortDevice::run()
{
    serialPort = new QSerialPort(this);
    serialPort->setPortName(_name);
    serialPort->setBaudRate(_baudRate);
    serialPort->setDataBits(_dataBits);
    serialPort->setStopBits(_stopBits);
    serialPort->setParity(_parity);


    connect(serialPort, &QSerialPort::readyRead, this, &SAKSerialPortDevice::readBytes);
    connect(qApp, &QApplication::lastWindowClosed, this, &SAKSerialPortDevice::terminate);

    if (serialPort->open(QSerialPort::ReadWrite)){
#ifdef QT_DEBUG
        qDebug() << serialPort->portName() << serialPort->baudRate() << serialPort->dataBits() << serialPort->stopBits() << serialPort->parity();
#endif
        emit deviceStatuChanged(true);
        exec();
    }else{        
        emit deviceStatuChanged(false);
        emit messageChanged(tr("串口打开失败：") + serialPort->errorString(), false);
        return;
    }
}

void SAKSerialPortDevice::readBytes()
{    
    serialPort->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
    QByteArray data = serialPort->readAll();    
    if (data.isEmpty()){
        return;
    }
    emit bytesRead(data);
}

void SAKSerialPortDevice::writeBytes(QByteArray data)
{    
    qint64 ret = serialPort->write(data);
    serialPort->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
    if (ret == -1){
        emit messageChanged(tr("串口发送数据失败：") + serialPort->errorString(), false);
    }else{
        emit bytesWriten(data);
    }
}

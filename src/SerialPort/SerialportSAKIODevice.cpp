/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "SerialportSAKIODevice.h"
#include <QApplication>

SerialportSAKIODevice::SerialportSAKIODevice(SAKDeviceType deviceType, QObject *parent):
    SAKIODevice(deviceType, parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

SerialportSAKIODevice::~SerialportSAKIODevice()
{

}

QByteArray SerialportSAKIODevice::ReadAll()
{
    QByteArray data = mpSerialPort->readAll();
    emit bytesRead(data);
    return data;
}

void SerialportSAKIODevice::writeBytes(QByteArray data)
{
    qint64 ret = mpSerialPort->write(data);
    if (ret == -1){
        qWarning() << "Write data failed:" << mpSerialPort->errorString();
    }else {
        mErrorString = "No error!";
        emit bytesWritten(ret);
    }
}

void SerialportSAKIODevice::open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity)
{
    if (mpSerialPort->isOpen()){
#if 0
        OutputInfo("Serial port is open. Nothing to do!", "red");
        mErrorString = "Serial port is open. Nothing to do!";
        return false;
#else
        mpSerialPort->close();
#endif
    }

    /// 设置串口名称
    mpSerialPort->setPortName(portName);
    /// 设置波特路
    bool isOK = false;
    mpSerialPort->setBaudRate(qint32(baudRate.toInt(&isOK, 10)));
    if (!isOK){
        OutputInfo("Invalid baud rate!", "red");
        mErrorString = "Invalid baud rate!";
        return;
    }
    /// 设置数据位
    if (dataBits.compare("8") == 0){
        mpSerialPort->setDataBits(QSerialPort::Data8);
    }else if (dataBits.compare("7") == 0){
        mpSerialPort->setDataBits(QSerialPort::Data7);
    }else if (dataBits.compare("6") == 0){
        mpSerialPort->setDataBits(QSerialPort::Data6);
    }else if (dataBits.compare("5") == 0){
        mpSerialPort->setDataBits(QSerialPort::Data5);
    }else{
        OutputInfo("Invalid baud rate!", "red");
        mErrorString = "Invalid baud rate!";
        return;
    }
    /// 设置停止位
    if (stopBits.compare("1") == 0){
        mpSerialPort->setStopBits(QSerialPort::OneStop);
    }else if (stopBits.compare("1.5") == 0){
        mpSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
    }else if (stopBits.compare("2") == 0){
        mpSerialPort->setStopBits(QSerialPort::TwoStop);
    }else {
        OutputInfo("Invalid stop bits!", "red");
        mErrorString = "Invalid stop bits!";
        return;
    }
    /// 设置奇偶校验位
    if (parity.compare("NoParity") == 0){
        mpSerialPort->setParity(QSerialPort::NoParity);
    }else if (parity.compare("EvenParity") == 0){
        mpSerialPort->setParity(QSerialPort::EvenParity);
    }else if (parity.compare("OddParity") == 0){
        mpSerialPort->setParity(QSerialPort::OddParity);
    }else if (parity.compare("SpaceParity") == 0){
        mpSerialPort->setParity(QSerialPort::SpaceParity);
    }else if (parity.compare("MarkParity") == 0){
        mpSerialPort->setParity(QSerialPort::MarkParity);
    }else {
        OutputInfo("Invalid parity!", "red");
        mErrorString = "Invalid parity!";
        return;
    }
    /// 打开串口
    if (mpSerialPort->open(QSerialPort::ReadWrite)){
        emit deviceOpenSuccessfully();
        qDebug() << isOpen();
        return;
    }else{
        QApplication::beep();
        emit errorStr(mpSerialPort->errorString());
        return;
    }
}

void SerialportSAKIODevice::close()
{
    mpSerialPort->close();
    emit deviceCloseSuccessfully();
}

void SerialportSAKIODevice::run()
{
     mpSerialPort = new QSerialPort;
    /// 读就绪信号关联
    connect(mpSerialPort, SIGNAL(readyRead()), this, SLOT(ReadAll()));
    this->exec();
}

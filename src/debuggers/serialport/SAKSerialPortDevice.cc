/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDevice::SAKSerialPortDevice(SAKSerialPortDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mSerialPort(Q_NULLPTR)
    ,mDebugPage(debugPage)
{

}

bool SAKSerialPortDevice::initializing(QString &errorString)
{
    errorString = tr("Unknown error");
    auto controller = qobject_cast<SAKSerialPortDeviceController*>(mDebugPage->deviceController());
    auto parameters = controller->parameters().value<SAKSerialPortDeviceController::SerialPortParameters>();
    mSerialPort = new QSerialPort;
    mSerialPort->setPortName(parameters.name);
    mSerialPort->setBaudRate(parameters.baudRate);
    mSerialPort->setDataBits(parameters.dataBits);
    mSerialPort->setStopBits(parameters.stopBits);
    mSerialPort->setParity(parameters.parity);
    mSerialPort->setFlowControl(parameters.flowControl);

    return true;
}

bool SAKSerialPortDevice::open(QString &errorString)
{
    if(mSerialPort->open(QSerialPort::ReadWrite)){
        errorString = tr("Unknown error");
        return true;
    }else{
        errorString = mSerialPort->errorString();
        return false;
    }
}

QByteArray SAKSerialPortDevice::read()
{
    QByteArray bytes = mSerialPort->readAll();
    return bytes;
}

QByteArray SAKSerialPortDevice::write(QByteArray bytes)
{
    qint64 ret = mSerialPort->write(bytes);
    if (ret > 0){
        return bytes;
    }else{
        return QByteArray();
    }
}

void SAKSerialPortDevice::close()
{
    mSerialPort->clear();
    mSerialPort->close();
}

void SAKSerialPortDevice::free()
{
    delete mSerialPort;
    mSerialPort = Q_NULLPTR;
}

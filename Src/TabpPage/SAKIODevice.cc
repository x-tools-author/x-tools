/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#include "SAKIODevice.hh"

SAKIODevice::SAKIODevice(QString deviceName, QObject *parent):QThread(parent)
{
    sakDeviceNmae = deviceName;
}


SAKIODevice::~SAKIODevice()
{

}

void SAKIODevice::open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort)
{
    Q_UNUSED(hostAddress);
    Q_UNUSED(hostPort);
    Q_UNUSED(peerAddress);
    Q_UNUSED(peerPort);
}

void SAKIODevice::open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity)
{
    Q_UNUSED(portName);
    Q_UNUSED(baudRate);
    Q_UNUSED(dataBits);
    Q_UNUSED(stopBits);
    Q_UNUSED(parity);
}

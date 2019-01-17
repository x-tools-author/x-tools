/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef TCPSAKIODEVICE_H
#define TCPSAKIODEVICE_H

#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QHostAddress>

#include "SAKIODevice.h"

class TcpSAKIODevice : public SAKIODevice
{
    Q_OBJECT
public:
    TcpSAKIODevice(QString deviceName = QString("TcpClient"), QObject *parent = Q_NULLPTR);
    ~TcpSAKIODevice();

    virtual bool isOpen();
    virtual QString errorString(){return mpTcpSocket->errorString();}
    virtual QString deviceName(){return QString("SAKDeviceTcp");}
public slots:
    virtual void writeBytes(QByteArray data);
    virtual void open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    virtual void close();

protected:
    void run();
private:
    QTcpSocket *mpTcpSocket = nullptr;
private slots:
    void readBytes();
    void afterDisConnect();
    void resetTheDevice();
};

#endif

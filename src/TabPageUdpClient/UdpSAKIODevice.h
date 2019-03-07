/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UDPSAKIODEVICE_H
#define UDPSAKIODEVICE_H

#include <QUdpSocket>
#include <QThread>
#include <QDebug>

#include "SAKIODevice.h"

class UdpSAKIODevice : public SAKIODevice
{
    Q_OBJECT
public:
    UdpSAKIODevice(QString deviceName = QString("UdpClient"), QObject *parent = Q_NULLPTR);
    ~UdpSAKIODevice();
    /// -------------------------------------------------
    virtual bool isOpen(){return mpUdpSocket->isOpen();}
    virtual QString errorString(){return mpUdpSocket->errorString();}
public slots:
    virtual void writeBytes(QByteArray data);
    virtual void open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    virtual void close();

    void readBytes();

protected:
    void run();
private:
    QString peerHostAddress;
    QString peerHostPort;
    QUdpSocket *mpUdpSocket = nullptr;
signals:
    /// 外部程序关联该信号，可以获取需要输出的信息
    void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};

#endif

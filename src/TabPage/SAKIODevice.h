/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKIODEVICE_H
#define SAKIODEVICE_H

#include <QObject>
#include <QThread>

class SAKIODevice : public QThread{
    Q_OBJECT
public:
    SAKIODevice(QString deviceName, QObject *parent = Q_NULLPTR);
    ~SAKIODevice();

    virtual bool isOpen(){return false;}
    virtual QString errorString(){return "Unknow device";}

    QString deviceName(){return sakDeviceNmae;}
public slots:
    virtual void writeBytes(QByteArray data){Q_UNUSED(data);}
    virtual void open(QString serverAddress, QString serverPort){Q_UNUSED(serverAddress);Q_UNUSED(serverPort);}
    virtual void open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    virtual void open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
    virtual void close(){}

protected:
    virtual void run(){}
private:
    /// SAKIODeviceSerialport、TcpClientSAKIODevice、TcpServerSAKIODevice、UdpSAKIODevice
    QString sakDeviceNmae;
signals:
    void deviceOpenSuccessfully();
    void deviceCloseSuccessfully();

    void bytesRead(QByteArray data);
    void bytesWritten(qint64);
    void errorStr(QString errorStr);
    void infoStr(QString informationStr);

    void peerHostInfo(QString host, quint16 port);
};

#endif

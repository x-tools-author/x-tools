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

    int readDelayTime(){return _readDelayTime;}
    int writeDelayTime(){return _writeDelayTime;}
    void setReadDelayTime(int t){t < 5 ? _readDelayTime = 5 : _readDelayTime = t;}
    void setWriteDelayTime(int t){t < 5 ? _writeDelayTime = 5 : _writeDelayTime = t;}
public slots:
    virtual void writeBytes(QByteArray data){Q_UNUSED(data);}
    virtual void open(QString serverAddress, QString serverPort){Q_UNUSED(serverAddress);Q_UNUSED(serverPort);}
    virtual void open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    virtual void open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
    virtual void close(){}

protected:
    virtual void run(){}

    int _readDelayTime = 5;
    int _writeDelayTime = 5;
private:
    /// SAKIODeviceSerialport、TcpClientSAKIODevice、TcpServerSAKIODevice、UdpSAKIODevice
    QString sakDeviceNmae;
signals:
    void deviceOpenSuccessfully();
    void deviceCloseSuccessfully();

    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void errorStr(QString errorStr);
    void infoStr(QString informationStr);

    void peerHostInfo(QString host, quint16 port);
};

#endif

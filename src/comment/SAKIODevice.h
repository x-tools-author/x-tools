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
    typedef enum __SAKDeviceType{
        SAKDeviceUnknow,
        SAKDeviceSerialport,
        SAKDeviceUdp,
        SAKDeviceTcp,
        SAKDeviceTcpServer
    }SAKDeviceType;

    SAKIODevice(SAKDeviceType deviceType = SAKDeviceUnknow, QObject *parent = Q_NULLPTR);
    ~SAKIODevice();

    virtual bool isOpen(){return false;}
    virtual QString errorString(){return "Unknow device";}
public slots:
    virtual void writeBytes(QByteArray data){Q_UNUSED(data);}
    virtual void open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    virtual void open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
    virtual void close(){}

    virtual SAKDeviceType deviceType(){return SAKDeviceUnknow;}

protected:
    virtual void run(){}
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

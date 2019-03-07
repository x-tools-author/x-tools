/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef TCPSERVERSAKIODEVICE_H
#define TCPSERVERSAKIODEVICE_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QDebug>
#include <QHostAddress>
#include <QTimer>

#include "SAKIODevice.h"

class TcpServerSAKIODevice : public SAKIODevice
{
    Q_OBJECT
public:
    TcpServerSAKIODevice(QString deviceName = QString("TcpServer"), QObject *parent = Q_NULLPTR);
    ~TcpServerSAKIODevice();

    virtual bool isOpen(){return mpTcpServer->isListening();}
    virtual QString errorString(){return mpTcpServer->errorString();}
public slots:
    virtual void writeBytes(QByteArray data);
    virtual void open(QString serverAddress, QString serverPort);
    virtual void close();

protected:
    void run();
private:
    QTimer *stateTimer              = nullptr;
    QTcpSocket *currentTcpClient    = nullptr;
    QTcpServer *mpTcpServer         = nullptr;
    QList <QTcpSocket *> clients;
private slots:
    void readBytes();
    void newConnecting();
    void checkState();
    void changedCurrentScoket(QString address, QString port);
signals:
    void clientsChanged(QList <QTcpSocket *> clients);
};

#define UI_STR_DEVICE_IS_NULL tr("设备指针为空!")

Q_DECLARE_METATYPE(QList <QTcpSocket *>)

#endif

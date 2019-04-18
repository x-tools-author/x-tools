/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKIODEVICECONTROLER_H
#define SAKIODEVICECONTROLER_H

#include <QWidget>

class SAKIODeviceControler:public QWidget{
    Q_OBJECT
public:
    SAKIODeviceControler(QWidget *parent = Q_NULLPTR);
    ~SAKIODeviceControler();

    virtual void afterDeviceOpen(){}
    virtual void afterDeviceClose(){}
public slots:
    virtual void open();
    virtual void refresh(){}
signals:
    void need2open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
    void need2open(QString hostAddress, QString hostPort, QString peerAddress, QString peerPort);
    void need2open(QString hostAddress, QString hostPort);
};

#endif

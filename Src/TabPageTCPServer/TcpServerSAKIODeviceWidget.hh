/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef TCPSERVERSAKIODEVICEWIDGET_HH
#define TCPSERVERSAKIODEVICEWIDGET_HH

#include "TcpServerSAKIODevice.hh"
#include "TcpServerSAKIODeviceControler.hh"
#include "SAKIODeviceWidget.hh"

class TcpServerSAKIODeviceWidget:public SAKIODeviceWidget
{
    Q_OBJECT
public:
    TcpServerSAKIODeviceWidget(TcpServerSAKIODevice *_device, TcpServerSAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~TcpServerSAKIODeviceWidget();
private:
    TcpServerSAKIODevice *device                = nullptr;
    TcpServerSAKIODeviceControler *controler    = nullptr;
private slots:

};

#endif


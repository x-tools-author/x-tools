/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef TCPSAKIODEVICEWIDGET_HH
#define TCPSAKIODEVICEWIDGET_HH

#include "TcpSAKIODevice.hh"
#include "TcpSAKIODeviceControler.hh"
#include "SAKIODeviceWidget.hh"

class TcpSAKIODeviceWidget:public SAKIODeviceWidget
{
    Q_OBJECT
public:
    TcpSAKIODeviceWidget(TcpSAKIODevice *_device, TcpSAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~TcpSAKIODeviceWidget();
private:
    TcpSAKIODevice *device = nullptr;
    TcpSAKIODeviceControler *controler = nullptr;
};

#endif

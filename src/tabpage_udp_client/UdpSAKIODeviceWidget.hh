/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UDPSAKIODEVICEWIDGET_HH
#define UDPSAKIODEVICEWIDGET_HH

#include "SAKIODeviceWidget.hh"
#include "UdpSAKIODevice.hh"
#include "UdpSAKIODeviceControler.hh"

class UdpSAKIODeviceWidget : public SAKIODeviceWidget
{
    Q_OBJECT
public:
    UdpSAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~UdpSAKIODeviceWidget();
};

#endif

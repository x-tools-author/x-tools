/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UDPSAKIODEVICEWIDGET_H
#define UDPSAKIODEVICEWIDGET_H

#include "SAKIODeviceWidget.hpp"
#include "UdpSAKIODevice.h"
#include "UdpSAKIODeviceControler.h"

class UdpSAKIODeviceWidget : public SAKIODeviceWidget
{
    Q_OBJECT
public:
    UdpSAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~UdpSAKIODeviceWidget();
};

#endif

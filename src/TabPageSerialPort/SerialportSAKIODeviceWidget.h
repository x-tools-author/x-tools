/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SERIALPORTSAKIODEVICEWIDGET_H
#define SERIALPORTSAKIODEVICEWIDGET_H

#include "SAKIODeviceWidget.hpp"
#include "SerialportSAKIODevice.h"
#include "SerialportSAKIODeviceControler.h"

class SerialportSAKIODeviceWidget : public SAKIODeviceWidget
{
    Q_OBJECT
public:
    SerialportSAKIODeviceWidget(SerialportSAKIODevice *_device, SerialportSAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~SerialportSAKIODeviceWidget();
public:
    SerialportSAKIODevice *device = nullptr;
    SerialportSAKIODeviceControler *controler = nullptr;
};

#endif  // SERIALPORTSAKIODEVICEWIDGET_H

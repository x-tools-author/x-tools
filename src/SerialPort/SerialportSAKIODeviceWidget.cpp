/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SerialportSAKIODeviceWidget.h"

SerialportSAKIODeviceWidget::SerialportSAKIODeviceWidget(SerialportSAKIODevice *_device, SerialportSAKIODeviceControler *_controler, QWidget *parent)
    :SAKIODeviceWidget(_device, _controler, parent)
    ,device(_device)
    ,controler(_controler)
{

}

SerialportSAKIODeviceWidget::~SerialportSAKIODeviceWidget()
{
    delete device;
    delete controler;
}

/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif


#include "UdpSAKIODeviceWidget.h"

UdpSAKIODeviceWidget::UdpSAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent)
    :SAKIODeviceWidget(_device, _controler, parent)
{

}

UdpSAKIODeviceWidget::~UdpSAKIODeviceWidget()
{

}

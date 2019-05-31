/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#include "TcpSAKIODeviceWidget.hh"

TcpSAKIODeviceWidget::TcpSAKIODeviceWidget(TcpSAKIODevice *_device, TcpSAKIODeviceControler *_controler, QWidget *parent)
    :SAKIODeviceWidget(_device, _controler, parent)
    ,device(_device)
    ,controler(_controler)
{
    connect(controler, SIGNAL(need2refresh()), device, SLOT(resetTheDevice()));
}

TcpSAKIODeviceWidget::~TcpSAKIODeviceWidget()
{

}

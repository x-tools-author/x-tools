/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "UdpServerUi.h"

UdpServerUi::UdpServerUi(xIO::DeviceType type, QWidget *parent)
    : SocketServerUi(type, parent)
{
    setClientWidgetsVisible(false);
    setChannelWidgetsVisible(false);
    setAuthenticationWidgetsVisible(false);
}

UdpServerUi::~UdpServerUi() {}

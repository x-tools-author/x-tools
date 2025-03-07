/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpserverui.h"

#include "udpserver.h"

UdpServerUi::UdpServerUi(QWidget *parent)
    : SocketServerUi(parent)
{
    setClientWidgetsVisible(false);
    setChannelWidgetsVisible(false);
    setAuthenticationWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
}

UdpServerUi::~UdpServerUi() {}

Device *UdpServerUi::newDevice()
{
    auto server = new UdpServer(this);
    setupServer(server);
    return server;
}

void UdpServerUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
}

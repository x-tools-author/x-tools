/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tcpserverui.h"

#include "tcpserver.h"

TcpServerUi::TcpServerUi(QWidget *parent)
    : SocketServerUi(parent)
{
    setClientWidgetsVisible(false);
    setChannelWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
    setAuthenticationWidgetsVisible(false);
}

TcpServerUi::~TcpServerUi() {}

Device *TcpServerUi::newDevice()
{
    auto server = new TcpServer(this);
    setupServer(server);
    return server;
}

void TcpServerUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
}

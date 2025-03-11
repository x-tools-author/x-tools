/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tcpclientui.h"

#include "tcpclient.h"

TcpClientUi::TcpClientUi(QWidget *parent)
    : SocketClientUi(parent)
{
    setWriteToWidgetsVisible(false);
    setChannelWidgetsVisible(false);
    setAuthenticationWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
}

TcpClientUi::~TcpClientUi() {}

Device *TcpClientUi::newDevice()
{
    return new TcpClient(this);
}

void TcpClientUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
}

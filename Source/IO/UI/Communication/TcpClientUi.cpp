/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TcpClientUi.h"

namespace xTools {

TcpClientUi::TcpClientUi(QWidget *parent)
    : SocketClientUi(xIO::CommunicationType::TcpClient, parent)
{
    setClientWidgetsVisible(false);
    setWriteToWidgetsVisible(false);
    setChannelWidgetsVisible(false);
    setAuthenticationWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
}

TcpClientUi::~TcpClientUi() {}

void TcpClientUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
}

} // namespace xTools

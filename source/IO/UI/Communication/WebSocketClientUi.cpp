﻿/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketClientUi.h"

namespace xTools {

WebSocketClientUi::WebSocketClientUi(QWidget *parent)
    : SocketClientUi(xIO::CommunicationType::WebSocketClient, parent)
{
    setClientWidgetsVisible(false);
    setWriteToWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
}

WebSocketClientUi::~WebSocketClientUi() {}

void WebSocketClientUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
    setAuthenticationWidgetsEnabled(enabled);
}

} // namespace xTools

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "websocketclientui.h"

#include "websocketclient.h"

WebSocketClientUi::WebSocketClientUi(QWidget *parent)
    : SocketClientUi(parent)
{
    setWriteToWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
#if 0
    setSecureModeWidgetsVisible(true);
#endif
}

WebSocketClientUi::~WebSocketClientUi() {}

Device *WebSocketClientUi::newDevice()
{
    return new WebSocketClient(this);
}

void WebSocketClientUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
    setAuthenticationWidgetsEnabled(enabled);
    setPathWidgetsEnabled(enabled);
}

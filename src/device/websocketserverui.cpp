/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "websocketserverui.h"

#include "websocketserver.h"

WebSocketServerUi::WebSocketServerUi(QWidget *parent)
    : SocketServerUi(parent)
{
    setAuthenticationWidgetsVisible(false);
    setMulticastWidgetsVisible(false);
    setPathWidgetsVisible(true);
}

WebSocketServerUi::~WebSocketServerUi() {}

Device *WebSocketServerUi::newDevice()
{
    auto server = new WebSocketServer(this);
    setupServer(server);
    return server;
}

void WebSocketServerUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
    setPathWidgetsEnabled(enabled);
}

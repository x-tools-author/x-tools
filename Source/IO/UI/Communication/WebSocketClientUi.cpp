/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketClientUi.h"

WebSocketClientUi::WebSocketClientUi(xIO::CommunicationType type, QWidget *parent)
    : SocketClientUi(type, parent)
{
    setClientWidgetsVisible(false);
    setWriteToWidgetsVisible(false);
}

WebSocketClientUi::~WebSocketClientUi() {}

void WebSocketClientUi::setUiEnabled(bool enabled)
{
    Q_UNUSED(enabled);
}

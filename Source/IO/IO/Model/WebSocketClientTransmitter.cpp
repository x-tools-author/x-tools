/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketClientTransmitter.h"

#include "../Communication/WebSocketClient.h"

namespace xTools {

WebSocketClientTransmitter::WebSocketClientTransmitter(QObject *parent)
    : SocketClientTransmitter{parent}
{}

Communication *WebSocketClientTransmitter::createTool()
{
    WebSocketClient *tool = new WebSocketClient();
    return tool;
}

} // namespace xTools

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "websocketservertransfermodel.h"

#include "device/websocketserver.h"

WebSocketServerTransferModel::WebSocketServerTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

WebSocketServerTransferModel::~WebSocketServerTransferModel() {}

Socket *WebSocketServerTransferModel::createSocket()
{
    return new WebSocketServer(this);
}

bool WebSocketServerTransferModel::isEnableRestartingColumn(int column) const
{
    if (column == 3 || column == 4) {
        return true;
    }

    return false;
}

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "websocketclienttransfermodel.h"

#include "device/websocketclient.h"

WebSocketClientTransferModel::WebSocketClientTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

WebSocketClientTransferModel::~WebSocketClientTransferModel() {}

Socket *WebSocketClientTransferModel::createSocket()
{
    return new WebSocketClient(this);
}

bool WebSocketClientTransferModel::isEnableRestartColumn(int column) const
{
    if (column == SOCKET_ROW_ADDRESS || column == SOCKET_ROW_PORT
        || column == SOCKET_ROW_AUTHENTICATION || column == SOCKET_ROW_USERNAME
        || column == SOCKET_ROW_PASSWORD) {
        return true;
    }

    return false;
}

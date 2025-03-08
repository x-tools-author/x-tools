/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wsclienttransfermodel.h"

#include "device/websocketclient.h"

WsClientTransferModel::WsClientTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

WsClientTransferModel::~WsClientTransferModel() {}

Socket *WsClientTransferModel::createSocket()
{
    return new WebSocketClient(this);
}

bool WsClientTransferModel::isEnableRestartColumn(int column) const
{
    if (column == 3 || column == 4) {
        return true;
    }

    return false;
}

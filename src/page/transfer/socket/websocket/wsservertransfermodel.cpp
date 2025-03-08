/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wsservertransfermodel.h"

#include "device/websocketserver.h"

WsServerTransferModel::WsServerTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

WsServerTransferModel::~WsServerTransferModel() {}

Socket *WsServerTransferModel::createSocket()
{
    return new WebSocketServer(this);
}

bool WsServerTransferModel::isEnableRestartColumn(int column) const
{
    if (column == 3 || column == 4) {
        return true;
    }

    return false;
}

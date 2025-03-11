/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tcpclienttransfermodel.h"

#include "device/tcpclient.h"

TcpClientTransferModel::TcpClientTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

TcpClientTransferModel::~TcpClientTransferModel() {}

Socket *TcpClientTransferModel::createSocket()
{
    return new TcpClient(this);
}

bool TcpClientTransferModel::isEnableRestartColumn(int column) const
{
    if (column == SOCKET_ROW_ADDRESS || column == SOCKET_ROW_PORT) {
        return true;
    }

    return false;
}

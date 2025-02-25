/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TcpServerTransferModel.h"

#include "device/tcpserver.h"

namespace xTools {

TcpServerTransferModel::TcpServerTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

TcpServerTransferModel::~TcpServerTransferModel() {}

Socket *TcpServerTransferModel::createSocket()
{
    return new TcpServer(this);
}

bool TcpServerTransferModel::isEnableRestartingColumn(int column) const
{
    if (column == 3 || column == 4) {
        return true;
    }

    return false;
}

} // namespace xTools

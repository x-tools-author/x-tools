/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "UdpServerTransferModel.h"

#include "../Communication/UdpClient.h"

namespace xTools {

UdpServerTransferModel::UdpServerTransferModel(QObject *parent)
    : SocketTransferModel(parent)
{}

UdpServerTransferModel::~UdpServerTransferModel() {}

Socket *UdpServerTransferModel::createSocket()
{
    return new UdpClient(this);
}

bool UdpServerTransferModel::isEnableRestartingColumn(int column) const
{
    if (column == 3 || column == 4) {
        return true;
    }

    return false;
}

} // namespace xTools

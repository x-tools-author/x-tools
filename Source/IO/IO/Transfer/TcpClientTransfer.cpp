/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TcpClientTransfer.h"

#include "TcpClientTransferModel.h"

namespace xTools {

TcpClientTransfer::TcpClientTransfer(QObject *parent)
    : SocketTransfer(parent)
    , m_model(new TcpClientTransferModel(this))

{}

TcpClientTransfer::~TcpClientTransfer() {}

QVariant TcpClientTransfer::tableModel()
{
    return QVariant::fromValue(m_model);
}

} // namespace xTools

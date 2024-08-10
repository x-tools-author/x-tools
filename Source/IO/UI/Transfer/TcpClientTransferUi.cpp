/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TcpClientTransferUi.h"

namespace xTools {

TcpClientTransferUi::TcpClientTransferUi(QWidget *parent)
    : SocketTransferUi{parent}
{
    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

TcpClientTransferUi::~TcpClientTransferUi() {}

void TcpClientTransferUi::setupIO(AbstractIO *io)
{
    SocketTransferUi::setupIO(io);

    auto tv = tableView();
    for (int i = 5; i < 10; i++) {
        tv->hideColumn(i);
    }
}

} // namespace xTools

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpclienttransferui.h"

namespace xTools {

UdpClientTransferUi::UdpClientTransferUi(QWidget *parent)
    : SocketTransferUi{parent}
{
    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

UdpClientTransferUi::~UdpClientTransferUi() {}

void UdpClientTransferUi::setupIO(AbstractIO *io)
{
    SocketTransferUi::setupIO(io);

    auto tv = tableView();
    tv->hideColumn(1);
    tv->hideColumn(2);
    tv->hideColumn(5);
    tv->hideColumn(6);
    tv->hideColumn(7);
    tv->hideColumn(8);
}

} // namespace xTools

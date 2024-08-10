/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TcpServerTransferUi.h"

namespace xTools {

TcpServerTransferUi::TcpServerTransferUi(QWidget *parent)
    : SocketTransferUi{parent}
{
    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

TcpServerTransferUi::~TcpServerTransferUi() {}

void TcpServerTransferUi::setupIO(AbstractIO *io)
{
    SocketTransferUi::setupIO(io);

    auto tv = tableView();

    tv->hideColumn(1);
    tv->hideColumn(2);
    for (int i = 5; i < 9; i++) {
        tv->hideColumn(i);
    }
}

} // namespace xTools

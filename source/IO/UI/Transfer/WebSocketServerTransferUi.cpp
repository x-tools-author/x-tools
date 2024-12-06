/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketServerTransferUi.h"

namespace xTools {

WebSocketServerTransferUi::WebSocketServerTransferUi(QWidget *parent)
    : SocketTransferUi{parent}
{
    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

WebSocketServerTransferUi::~WebSocketServerTransferUi() {}

void WebSocketServerTransferUi::setupIO(AbstractIO *io)
{
    SocketTransferUi::setupIO(io);

    auto tv = tableView();
    tv->hideColumn(1);
    tv->hideColumn(2);
    for (int i = 6; i < 9; ++i) {
        tv->hideColumn(i);
    }
}

} // namespace xTools

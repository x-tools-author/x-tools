/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "websocketclienttransferview.h"

#include "websocketclienttransfermodel.h"

WebSocketClientTransferView::WebSocketClientTransferView(QWidget *parent)
    : SocketTransferView{parent}
{
    auto model = new WebSocketClientTransferModel(this);
    setTableModel(model);
    connect(model,
            &WebSocketClientTransferModel::outputBytes,
            this,
            &WebSocketClientTransferView::outputBytes);

    auto tv = tableView();
    tv->hideColumn(1);
    tv->hideColumn(2);
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

WebSocketClientTransferView::~WebSocketClientTransferView() {}

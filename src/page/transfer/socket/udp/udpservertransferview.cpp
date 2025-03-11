/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpservertransferview.h"

#include "udpservertransfermodel.h"

UdpServerTransferView::UdpServerTransferView(QWidget *parent)
    : SocketTransferView{parent}
{
    auto model = new UdpServerTransferModel(this);
    setTableModel(model);
    connect(model, &SocketTransferModel::outputBytes, this, &UdpServerTransferView::outputBytes);

    auto tv = tableView();
    tv->hideColumn(SOCKET_ROW_CHANNEL);
    tv->hideColumn(SOCKET_ROW_USERNAME);
    tv->hideColumn(SOCKET_ROW_PASSWORD);
    tv->hideColumn(SOCKET_ROW_AUTHENTICATION);
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

UdpServerTransferView::~UdpServerTransferView() {}

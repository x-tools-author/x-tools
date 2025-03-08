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
    tv->hideColumn(1);
    tv->hideColumn(2);
    tv->hideColumn(5);
    tv->hideColumn(6);
    tv->hideColumn(7);
    tv->hideColumn(8);
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

UdpServerTransferView::~UdpServerTransferView() {}

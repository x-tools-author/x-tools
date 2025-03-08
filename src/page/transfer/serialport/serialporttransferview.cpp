/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialporttransferview.h"

#include "serialporttransferdelegate.h"
#include "serialporttransfermodel.h"

SerialPortTransferView::SerialPortTransferView(QWidget *parent)
    : TransferView{parent}
{
    m_model = new SerialPortTransferModel(this);
    setTableModel(m_model);

    auto *tv = tableView();
    tv->setItemDelegate(new SerialPortTransferDelegate(this));
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

SerialPortTransferView::~SerialPortTransferView() {}

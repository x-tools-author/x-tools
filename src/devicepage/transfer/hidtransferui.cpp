/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hidtransferui.h"

#include <QHeaderView>

#include "hidstyleditemdelegate.h"

namespace xTools {

HidTransferUi::HidTransferUi(QWidget *parent)
    : AbstractTransferUi{parent}
{
    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

HidTransferUi::~HidTransferUi() {}

QStyledItemDelegate *HidTransferUi::createItemDelegate() const
{
    return new HidStyledItemDelegate();
}

} // namespace xTools

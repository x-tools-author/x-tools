/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractTransfer.h"

#include "AbstractTransferModel.h"

namespace xTools {

AbstractTransfer::AbstractTransfer(QObject *parent)
    : AbstractModelIO{parent}
{}

AbstractTransfer::~AbstractTransfer() {}

void AbstractTransfer::inputBytes(const QByteArray &bytes)
{
    if (!m_enable) {
        return;
    }

    AbstractTransferModel *model = tableModel().value<AbstractTransferModel *>();
    if (model) {
        model->inputBytes(bytes);
    }
}

} // namespace xTools

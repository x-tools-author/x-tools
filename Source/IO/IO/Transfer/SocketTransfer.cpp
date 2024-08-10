/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketTransfer.h"

namespace xTools {

SocketTransfer::SocketTransfer(QObject* parent)
    : AbstractTransfer(parent)
{}

SocketTransfer::~SocketTransfer() {}

void SocketTransfer::inputBytes(const QByteArray &bytes) {}

QVariantMap SocketTransfer::saveItem(const int row) const
{
    return QVariantMap{};
}

void SocketTransfer::loadItem(const int row, const QVariantMap &item) {}

} // namespace xTools

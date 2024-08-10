/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPortTransfer.h"

#include "SerialPortTransferModel.h"

namespace xTools {

SerialPortTransfer::SerialPortTransfer(QObject *parent)
    : AbstractTransfer{parent}
    , m_model{new SerialPortTransferModel{this}}
{}

SerialPortTransfer::~SerialPortTransfer() {}

void SerialPortTransfer::inputBytes(const QByteArray &bytes) {}

QVariant SerialPortTransfer::tableModel()
{
    return QVariant::fromValue(m_model);
}

QVariantMap SerialPortTransfer::saveItem(const int row) const
{
    Q_UNUSED(row)
    return QVariantMap{};
}

void SerialPortTransfer::loadItem(const int row, const QVariantMap &item)
{
    Q_UNUSED(row)
    Q_UNUSED(item)
}

} // namespace xTools

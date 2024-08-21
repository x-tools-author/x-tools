/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractTransferModel.h"

#include "../../IO/Communication/Communication.h"

namespace xTools {

AbstractTransferModel::AbstractTransferModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

AbstractTransferModel::~AbstractTransferModel() {}

int AbstractTransferModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_transfers.size();
}

bool AbstractTransferModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto transfer = createTransfer();
        connect(transfer, &Communication::outputBytes, this, &AbstractTransferModel::outputBytes);
        m_transfers.insert(row, {transfer, tr("Transfer %1").arg(row)});
    }
    endInsertRows();
    return true;
}

void AbstractTransferModel::inputBytes(const QByteArray &bytes)
{
    for (auto &item : m_transfers) {
        if (item.transfer->isEnable()) {
            item.transfer->inputBytes(bytes);
        }
    }
}

void AbstractTransferModel::startAll()
{
    for (auto &item : m_transfers) {
        if (item.transfer->isEnable()) {
            item.transfer->start();
        }
    }
}

void AbstractTransferModel::stopAll()
{
    for (auto &item : m_transfers) {
        item.transfer->exit();
        item.transfer->wait();
    }
}

bool AbstractTransferModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto tmp = m_transfers.takeAt(row);
        tmp.transfer->exit();
        tmp.transfer->wait();
        tmp.transfer->deleteLater();
    }
    endRemoveRows();
    return true;
}

} // namespace xTools

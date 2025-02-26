/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "abstracttransfermodel.h"

#include <QEventLoop>
#include <QTimer>

#include "device/communication.h"
#include "devicepage/common/xio.h"

namespace xTools {

AbstractTransferModel::AbstractTransferModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(this, &AbstractTransferModel::dataChanged, this, &AbstractTransferModel::onDataChanged);
}

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
        connect(transfer, &Communication::outputBytes, this, [=](const QByteArray &bytes) {
            for (auto &transferItem : this->m_transfers) {
                if (transferItem.option == static_cast<int>(xIO::TransferType::Bidirectional)) {
                    emit outputBytes(bytes);
                }
            }
        });
        connect(transfer, &Communication::finished, this, [=]() {
            if (this->m_enableRestartTransfer) {
                transfer->start();
            }
        });

        int option = static_cast<int>(xIO::TransferType::Bidirectional);
        m_transfers.insert(row + i, {transfer, tr("Transfer %1").arg(row), option});
        if (m_enableRestartTransfer) {
            transfer->start();
        }
    }
    endInsertRows();
    return true;
}

bool AbstractTransferModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto tmp = m_transfers.takeAt(row);
        disconnect(tmp.transfer, nullptr, nullptr, nullptr);
        tmp.transfer->exit();
        tmp.transfer->wait();
        tmp.transfer->deleteLater();
    }
    endRemoveRows();
    return true;
}

void AbstractTransferModel::inputBytes(const QByteArray &bytes)
{
    for (auto &item : m_transfers) {
        if (item.option != static_cast<int>(xIO::TransferType::Disabled)) {
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
    m_enableRestartTransfer = false;
    for (auto &item : m_transfers) {
        item.transfer->exit();
        item.transfer->wait();
    }
}

void AbstractTransferModel::setEnableRestartTransfer(bool enable)
{
    m_enableRestartTransfer = enable;
}

bool AbstractTransferModel::isEnableRestartingColumn(int column) const
{
    return false;
}

void AbstractTransferModel::onDataChanged(const QModelIndex &topLeft,
                                          const QModelIndex &bottomRight,
                                          const QList<int> &roles)
{
    if (topLeft != bottomRight) {
        return;
    }

    if (!isEnableRestartingColumn(topLeft.column())) {
        return;
    }

    if (!m_enableRestartTransfer) {
        return;
    }

    auto row = topLeft.row();
    if (row >= 0 && row < m_transfers.size()) {
        auto transfer = m_transfers.at(row).transfer;
        transfer->exit();
        transfer->wait();

        QEventLoop *loop = new QEventLoop(this);
        QTimer::singleShot(1000, this, [loop]() { loop->exit(); });
        loop->exec();
        transfer->start();
    }
}

} // namespace xTools

/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "transfermodel.h"

#include <QEventLoop>
#include <QTimer>

#include "common/xtools.h"
#include "device/device.h"

TransferModel::TransferModel(QObject *parent)
    : TableModel(parent)
{
    connect(this, &TransferModel::dataChanged, this, &TransferModel::onDataChanged);
}

TransferModel::~TransferModel() {}

int TransferModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_transfers.size();
}

bool TransferModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        auto transfer = createTransfer();
        connect(transfer, &Device::bytesRead, this, [=](const QByteArray &bytes, const QString &) {
            for (auto &transferItem : this->m_transfers) {
                if (transferItem.option == static_cast<int>(TransferType::Bidirectional)) {
                    emit outputBytes(bytes);
                }
            }
        });
        connect(transfer, &Device::bytesRead, this, &TransferModel::bytesRead);
        connect(transfer, &Device::bytesWritten, this, &TransferModel::bytesWritten);
        connect(transfer, &Device::finished, this, [=]() { transfer->start(); });

        int option = static_cast<int>(TransferType::Bidirectional);
        m_transfers.insert(row + i, {transfer, tr("Transfer %1").arg(row), option});
        transfer->start();
    }
    endInsertRows();
    return true;
}

bool TransferModel::removeRows(int row, int count, const QModelIndex &parent)
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

void TransferModel::inputBytes(const QByteArray &bytes)
{
    for (auto &item : m_transfers) {
        if (item.option != static_cast<int>(TransferType::Disabled)) {
            item.transfer->writeBytes(bytes);
        }
    }
}

void TransferModel::startAll()
{
    m_enableRestart = true;
    for (auto &item : m_transfers) {
        if (item.isEnable) {
            item.transfer->start();
        }
    }
}

void TransferModel::stopAll()
{
    m_enableRestart = false;
    for (auto &item : m_transfers) {
        item.transfer->exit();
        item.transfer->wait();
    }
}

bool TransferModel::isEnableRestartColumn(int column) const
{
    Q_UNUSED(column);
    return false;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void TransferModel::onDataChanged(const QModelIndex &topLeft,
                                  const QModelIndex &bottomRight,
                                  const QList<int> &roles)
#else
void TransferModel::onDataChanged(const QModelIndex &topLeft,
                                  const QModelIndex &bottomRight,
                                  const QVector<int> &roles)
#endif
{
    Q_UNUSED(roles);
    if (topLeft != bottomRight) {
        return;
    }

    if (!isEnableRestartColumn(topLeft.column())) {
        return;
    }

    if (!m_enableRestart) {
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

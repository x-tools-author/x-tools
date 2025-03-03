/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hidtransfer.h"

#include "common/xtools.h"
#include "hidtransfermodel.h"

HidTransfer::HidTransfer(QObject *parent)
    : AbstractTransfer{parent}
    , m_model{new HidTransferModel{this}}
{}

HidTransfer::~HidTransfer() {}

void HidTransfer::inputBytes(const QByteArray &bytes)
{
    if (!m_enable) {
        return;
    }

    m_model->inputBytes(bytes);
}

QVariant HidTransfer::tableModel() const
{
    return QVariant::fromValue(m_model);
}

QVariantMap HidTransfer::saveItem(const int row) const
{
    if (row < 0 || row >= m_model->rowCount()) {
        return {};
    }

    HidItem item;
    item.portName = m_model->data(m_model->index(row, 1), Qt::EditRole).toString();
    item.baudRate = m_model->data(m_model->index(row, 2), Qt::EditRole).toInt();
    item.dataBits = m_model->data(m_model->index(row, 3), Qt::EditRole).toInt();
    item.stopBits = m_model->data(m_model->index(row, 4), Qt::EditRole).toInt();
    item.parity = m_model->data(m_model->index(row, 5), Qt::EditRole).toInt();
    item.flowControl = m_model->data(m_model->index(row, 6), Qt::EditRole).toInt();

    QJsonObject obj = saveHidItem(item);
    obj.insert("enable", m_model->data(m_model->index(row, 0), Qt::EditRole).toBool());
    obj.insert("description", m_model->data(m_model->index(row, 7), Qt::EditRole).toString());

    return obj.toVariantMap();
}

void HidTransfer::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_model->rowCount()) {
        return;
    }

    bool enable = item.value("enable").toBool();
    QString description = item.value("description").toString();
    HidItem HidItem = loadHidItem(QJsonObject::fromVariantMap(item));

    m_model->setData(m_model->index(row, 0), enable, Qt::EditRole);
    m_model->setData(m_model->index(row, 1), HidItem.portName, Qt::EditRole);
    m_model->setData(m_model->index(row, 2), HidItem.baudRate, Qt::EditRole);
    m_model->setData(m_model->index(row, 3), HidItem.dataBits, Qt::EditRole);
    m_model->setData(m_model->index(row, 4), HidItem.stopBits, Qt::EditRole);
    m_model->setData(m_model->index(row, 5), HidItem.parity, Qt::EditRole);
    m_model->setData(m_model->index(row, 6), HidItem.flowControl, Qt::EditRole);
    m_model->setData(m_model->index(row, 7), description, Qt::EditRole);
}

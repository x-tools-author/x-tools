/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialporttransfer.h"

#include "devicepage/common/xio.h"
#include "serialporttransfermodel.h"

namespace xTools {

SerialPortTransfer::SerialPortTransfer(QObject *parent)
    : AbstractTransfer{parent}
    , m_model{new SerialPortTransferModel{this}}
{
    connect(m_model, &AbstractTransferModel::outputBytes, this, &AbstractTransfer::outputBytes);
}

SerialPortTransfer::~SerialPortTransfer() {}

void SerialPortTransfer::inputBytes(const QByteArray &bytes)
{
    if (!m_enable) {
        return;
    }

    m_model->inputBytes(bytes);
}

QVariant SerialPortTransfer::tableModel() const
{
    return QVariant::fromValue(m_model);
}

QVariantMap SerialPortTransfer::saveItem(const int row) const
{
    if (row < 0 || row >= m_model->rowCount()) {
        return {};
    }

    xIO::SerialPortItem item;
    item.portName = m_model->data(m_model->index(row, 1), Qt::EditRole).toString();
    item.baudRate = m_model->data(m_model->index(row, 2), Qt::EditRole).toInt();
    item.dataBits = m_model->data(m_model->index(row, 3), Qt::EditRole).toInt();
    item.stopBits = m_model->data(m_model->index(row, 4), Qt::EditRole).toInt();
    item.parity = m_model->data(m_model->index(row, 5), Qt::EditRole).toInt();
    item.flowControl = m_model->data(m_model->index(row, 6), Qt::EditRole).toInt();

    QJsonObject obj = xIO::saveSerialPortItem(item);
    obj.insert("enable", m_model->data(m_model->index(row, 0), Qt::EditRole).toBool());
    obj.insert("description", m_model->data(m_model->index(row, 7), Qt::EditRole).toString());

    return obj.toVariantMap();
}

void SerialPortTransfer::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_model->rowCount()) {
        return;
    }

    bool enable = item.value("enable").toBool();
    QString description = item.value("description").toString();
    xIO::SerialPortItem serialPortItem = xIO::loadSerialPortItem(QJsonObject::fromVariantMap(item));

    m_model->setData(m_model->index(row, 0), enable, Qt::EditRole);
    m_model->setData(m_model->index(row, 1), serialPortItem.portName, Qt::EditRole);
    m_model->setData(m_model->index(row, 2), serialPortItem.baudRate, Qt::EditRole);
    m_model->setData(m_model->index(row, 3), serialPortItem.dataBits, Qt::EditRole);
    m_model->setData(m_model->index(row, 4), serialPortItem.stopBits, Qt::EditRole);
    m_model->setData(m_model->index(row, 5), serialPortItem.parity, Qt::EditRole);
    m_model->setData(m_model->index(row, 6), serialPortItem.flowControl, Qt::EditRole);
    m_model->setData(m_model->index(row, 7), description, Qt::EditRole);
}

} // namespace xTools

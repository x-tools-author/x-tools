/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sockettransfer.h"

#include "common/xtools.h"
#include "sockettransfermodel.h"

SocketTransfer::SocketTransfer(QObject *parent)
    : AbstractTransfer(parent)
{}

SocketTransfer::~SocketTransfer() {}

QVariantMap SocketTransfer::saveItem(const int row) const
{
    QVariant modelVar = tableModel();
    QObject *modelObj = modelVar.value<QObject *>();
    auto *model = qobject_cast<SocketTransferModel *>(modelObj);
    if (model == nullptr) {
        qWarning() << "The model is not a SocketTransferModel";
        return {};
    }

    if (row < 0 || row >= model->rowCount()) {
        return {};
    }

    SocketItem item;
    item.clientAddress = model->data(model->index(row, 1), Qt::EditRole).toString();
    item.clientPort = model->data(model->index(row, 2), Qt::EditRole).toInt();
    item.serverAddress = model->data(model->index(row, 3), Qt::EditRole).toString();
    item.serverPort = model->data(model->index(row, 4), Qt::EditRole).toInt();

    int channel = model->data(model->index(row, 5), Qt::EditRole).toInt();
    auto cookedChannel = static_cast<WebSocketDataChannel>(channel);
    item.dataChannel = cookedChannel;

    item.authentication = model->data(model->index(row, 6), Qt::EditRole).toBool();
    item.username = model->data(model->index(row, 7), Qt::EditRole).toString();
    item.password = model->data(model->index(row, 8), Qt::EditRole).toString();

    QJsonObject obj = saveSocketItem(item);
    obj.insert("enable", model->data(model->index(row, 0), Qt::EditRole).toBool());
    obj.insert("description", model->data(model->index(row, 9), Qt::EditRole).toString());

    return obj.toVariantMap();
}

void SocketTransfer::loadItem(const int row, const QVariantMap &item)
{
    QVariant modelVar = tableModel();
    QObject *modelObj = modelVar.value<QObject *>();
    auto *model = qobject_cast<SocketTransferModel *>(modelObj);
    if (model == nullptr) {
        qWarning() << "The value of model is nullptr";
        return;
    }

    if (row < 0 || row >= model->rowCount()) {
        return;
    }

    SocketItem socketItem = loadSocketItem(QJsonObject::fromVariantMap(item));
    model->setData(model->index(row, 0), item.value("enable").toBool(), Qt::EditRole);
    model->setData(model->index(row, 1), socketItem.clientAddress, Qt::EditRole);
    model->setData(model->index(row, 2), socketItem.clientPort, Qt::EditRole);
    model->setData(model->index(row, 3), socketItem.serverAddress, Qt::EditRole);
    model->setData(model->index(row, 4), socketItem.serverPort, Qt::EditRole);
    model->setData(model->index(row, 5), static_cast<int>(socketItem.dataChannel), Qt::EditRole);
    model->setData(model->index(row, 6), socketItem.authentication, Qt::EditRole);
    model->setData(model->index(row, 7), socketItem.username, Qt::EditRole);
    model->setData(model->index(row, 8), socketItem.password, Qt::EditRole);
    model->setData(model->index(row, 9), item.value("description").toString(), Qt::EditRole);

    emit model->dataChanged(model->index(row, 0), model->index(row, 9));
}

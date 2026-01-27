/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapmsgmodel.h"

#include <QMutex>

#include <coap3/coap.h>

namespace xCoAP {

class CoAPMsgModelPrivate : public QObject
{
public:
    CoAPMsgModelPrivate(CoAPMsgModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPMsgModelPrivate() override {}

public:
    QList<std::shared_ptr<CoAPMsgItem>> m_msgList;
    QMutex m_msgListMutex;

private:
    CoAPMsgModel* q{nullptr};
};

CoAPMsgModel::CoAPMsgModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPMsgModelPrivate(this);
}

CoAPMsgModel::~CoAPMsgModel() {}

void CoAPMsgModel::addRow(std::shared_ptr<CoAPMsgItem> request,
                          std::shared_ptr<CoAPMsgItem> response)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    d->m_msgListMutex.lock();
    d->m_msgList.append(request);
    d->m_msgListMutex.unlock();
    endInsertRows();

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    d->m_msgListMutex.lock();
    d->m_msgList.append(response);
    d->m_msgListMutex.unlock();
    endInsertRows();
}

void CoAPMsgModel::clear()
{
    beginResetModel();
    d->m_msgListMutex.lock();
    for (auto& item : d->m_msgList) {
        item.reset();
    }
    d->m_msgList.clear();
    d->m_msgListMutex.unlock();
    endResetModel();
}

QVariant CoAPMsgModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    std::shared_ptr<CoAPMsgItem> item = d->m_msgList.at(row);
    if (role == Qt::DisplayRole) {
        if (column == CO_AP_MODEL_COLUMN_FLAG) {
            return item->isRx ? tr("Rx") : tr("Tx");
        } else if (column == CO_AP_MODEL_COLUMN_MESSAGE_ID) {
            return QString::number(item->messageId);
        } else if (column == CO_AP_MODEL_COLUMN_HOST_LOCAL) {
            return item->clientHost;
        } else if (column == CO_AP_MODEL_COLUMN_HOST_REMOTE) {
            return item->serverHost;
        } else if (column == CO_AP_MODEL_COLUMN_VERSION) {
            return QString::number(item->version);
        } else if (column == CO_AP_MODEL_COLUMN_TYPE) {
            return item->type;
        } else if (column == CO_AP_MODEL_COLUMN_TOKEN_LENGTH) {
            return QString::number(item->tokenLength);
        } else if (column == CO_AP_MODEL_COLUMN_TOKEN) {
            return QString::fromLatin1(item->token.toHex(' '));
        } else if (column == CO_AP_MODEL_COLUMN_OPTIONS) {
            return QString::fromLatin1(item->options.toHex(' '));
        } else if (column == CO_AP_MODEL_COLUMN_PAYLOAD) {
            return QString::fromLatin1(item->payload.toHex(' '));
        }

        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        if (column == CO_AP_MODEL_COLUMN_PAYLOAD) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }

        return int(Qt::AlignCenter);
    }

    return QVariant();
}

int CoAPMsgModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    d->m_msgListMutex.lock();
    int count = d->m_msgList.count();
    d->m_msgListMutex.unlock();
    return count;
}

int CoAPMsgModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 10;
}

QVariant CoAPMsgModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CO_AP_MODEL_COLUMN_FLAG:
                return tr("Flag");
            case CO_AP_MODEL_COLUMN_MESSAGE_ID:
                return tr("Message ID");
            case CO_AP_MODEL_COLUMN_HOST_LOCAL:
                return tr("Client");
            case CO_AP_MODEL_COLUMN_HOST_REMOTE:
                return tr("Server");
            case CO_AP_MODEL_COLUMN_VERSION:
                return tr("Version");
            case CO_AP_MODEL_COLUMN_TYPE:
                return tr("Type");
            case CO_AP_MODEL_COLUMN_TOKEN_LENGTH:
                return tr("Token Length");
            case CO_AP_MODEL_COLUMN_TOKEN:
                return tr("Token");
            case CO_AP_MODEL_COLUMN_OPTIONS:
                return tr("Options");
            case CO_AP_MODEL_COLUMN_PAYLOAD:
                return tr("Payload");
            default:
                return QVariant();
            }
        }
    } else {
        Q_UNUSED(role);
        return QString::number(section + 1);
    }

    return QVariant();
}

} // namespace xCoAP

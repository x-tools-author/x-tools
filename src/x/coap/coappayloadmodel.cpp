/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadmodel.h"

#include <QMutex>

#include <coap3/coap.h>

#include "coapcommon.h"

namespace xCoAP {

class CoAPPayloadModelPrivate : public QObject
{
public:
    CoAPPayloadModelPrivate(CoAPPayloadModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPPayloadModelPrivate() override {}

public:
    QList<std::shared_ptr<CoAPCommon::PayloadContext>> m_payloads;

private:
    CoAPPayloadModel* q{nullptr};
};

CoAPPayloadModel::CoAPPayloadModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPPayloadModelPrivate(this);
}

CoAPPayloadModel::~CoAPPayloadModel() {}

void CoAPPayloadModel::addPayload(const QJsonObject& context)
{
    CoAPCommon::PayloadContext ctx = CoAPCommon::jsonObject2PayloadContext(context);
    auto payloadPtr = std::make_shared<CoAPCommon::PayloadContext>(ctx);

    beginInsertRows(QModelIndex(), d->m_payloads.size(), d->m_payloads.size());
    d->m_payloads.append(payloadPtr);
    endInsertRows();
    emit dataChanged(index(0, 0),
                     index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1));
}

void CoAPPayloadModel::clearPayloads()
{
    beginResetModel();
    d->m_payloads.clear();
    endResetModel();
}

void CoAPPayloadModel::updateRow(int row, const QJsonObject& context)
{
    if (row < 0 || row >= d->m_payloads.size()) {
        return;
    }

    CoAPCommon::PayloadContext ctx = CoAPCommon::jsonObject2PayloadContext(context);
    d->m_payloads[row]->description = ctx.description;
    d->m_payloads[row]->format = ctx.format;
    d->m_payloads[row]->data = ctx.data;
    emit dataChanged(index(row, 0), index(row, columnCount(QModelIndex()) - 1));
}

QVariant CoAPPayloadModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    if (row < 0 || row >= d->m_payloads.size()) {
        return QVariant();
    }

    if (column < 0 || column >= columnCount(QModelIndex())) {
        return QVariant();
    }

    if (role == CO_AP_PAYLOAD_DATA_ROLE_PAYLOAD) {
        const std::shared_ptr<CoAPCommon::PayloadContext>& payload = d->m_payloads.at(index.row());
        return QVariant::fromValue(payload);
    }

    if (role == Qt::DisplayRole) {
        const auto& payload = d->m_payloads.at(index.row());
        switch (index.column()) {
        case CO_AP_PAYLOAD_MODEL_COLUMN_DESCRIPTION:
            return payload->description;
        case CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT:
            return payload->format;
        case CO_AP_PAYLOAD_MODEL_COLUMN_DATA:
            return QString::fromUtf8(payload->data);
        default:
            return QVariant();
        }
    }

    if (role == Qt::EditRole) {
        const auto& payload = d->m_payloads.at(index.row());
        switch (index.column()) {
        case CO_AP_PAYLOAD_MODEL_COLUMN_DESCRIPTION:
            return payload->description;
        case CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT:
            return payload->format;
        case CO_AP_PAYLOAD_MODEL_COLUMN_DATA:
            return QString::fromUtf8(payload->data);
        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (column == CO_AP_PAYLOAD_MODEL_COLUMN_DATA) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter);
        }
    }

    return QVariant();
}

int CoAPPayloadModel::rowCount(const QModelIndex& parent) const
{
    return d->m_payloads.size();
}

int CoAPPayloadModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CoAPPayloadModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CO_AP_PAYLOAD_MODEL_COLUMN_DESCRIPTION:
                return tr("Description");
            case CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT:
                return tr("Format");
            case CO_AP_PAYLOAD_MODEL_COLUMN_DATA:
                return tr("Data");
            default:
                return QVariant();
            }
        }

        if (role == Qt::TextAlignmentRole) {
            if (section == CO_AP_PAYLOAD_MODEL_COLUMN_DATA) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            } else {
                return int(Qt::AlignCenter);
            }
        }
    } else {
        if (role == Qt::DisplayRole) {
            return QString::number(section + 1);
        }
    }

    return QVariant();
}

bool CoAPPayloadModel::removeRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(parent);
    if (row < 0 || row >= d->m_payloads.size() || count <= 0
        || (row + count) > d->m_payloads.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        d->m_payloads.removeAt(row);
    }
    endRemoveRows();
    return true;
}

} // namespace xCoAP

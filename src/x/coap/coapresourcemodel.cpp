/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourcemodel.h"

#include "coapcommon.h"

namespace xCoAP {

class CoAPResourceModelPrivate : public QObject
{
public:
    CoAPResourceModelPrivate(CoAPResourceModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPResourceModelPrivate() override {}

public:
    QList<CoAPCommon::ResourceItem> m_resourceItems;

private:
    CoAPResourceModel* q{nullptr};
};

CoAPResourceModel::CoAPResourceModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPResourceModelPrivate(this);
}

CoAPResourceModel::~CoAPResourceModel() {}

void CoAPResourceModel::addResource(const QJsonObject& obj)
{
    beginInsertRows(QModelIndex(), d->m_resourceItems.count(), d->m_resourceItems.count());
    CoAPCommon::ResourceItem item = CoAPCommon::jsonObject2ResourceItem(obj);
    d->m_resourceItems.append(item);
    endInsertRows();
}

void CoAPResourceModel::updateResource(int row, const QJsonObject& obj)
{
    if (row < 0 && row > d->m_resourceItems.count() - 1) {
        qCDebug(xCoAPLog) << "Invalid row:" << row;
        return;
    }
    d->m_resourceItems[row] = CoAPCommon::jsonObject2ResourceItem(obj);
    emit dataChanged(index(row, 0), index(row, columnCount(QModelIndex()) - 1));
}

QJsonObject CoAPResourceModel::saveRow(int row) const
{
    if (row < 0 && row > d->m_resourceItems.count() - 1) {
        qCDebug(xCoAPLog) << "Invalid row:" << row;
        return QJsonObject();
    }
    return CoAPCommon::resourceItem2JsonObject(d->m_resourceItems.at(row));
}

void CoAPResourceModel::deleteRow(int row)
{
    if (row < 0 && row > d->m_resourceItems.count() - 1) {
        qCDebug(xCoAPLog) << "Invalid row:" << row;
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    d->m_resourceItems.removeAt(row);
    endRemoveRows();
}

void CoAPResourceModel::clearAllRows()
{
    beginResetModel();
    d->m_resourceItems.clear();
    endResetModel();
}

QStringList CoAPResourceModel::uriPaths() const
{
    QStringList tmp;
    for (const auto& item : std::as_const(d->m_resourceItems)) {
        tmp.append(item.uriPath);
    }
    return tmp;
}

int CoAPResourceModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return d->m_resourceItems.count();
}

int CoAPResourceModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant CoAPResourceModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    if (row < 0 || row >= d->m_resourceItems.count()) {
        return QVariant();
    }

    int column = index.column();
    if (column < 0 || columnCount(QModelIndex()) <= column) {
        return QVariant();
    }

    const auto& item = d->m_resourceItems.at(row);
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case COAP_RES_MODEL_COLUMN_DESCRIPTION:
            return item.description;
        case COAP_RES_MODEL_COLUMN_URI:
            return item.uriPath;
        case COAP_RES_MODEL_COLUMN_FORMAT:
            return CoAPCommon::getContextFormatString(item.contextFormat);
        case COAP_RES_MODEL_COLUMN_PAYLOAD:
            return QString::fromLatin1(item.payload.toHex());
        default:
            return QVariant();
        }
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case COAP_RES_MODEL_COLUMN_DESCRIPTION:
            return item.description;
        case COAP_RES_MODEL_COLUMN_URI:
            return item.uriPath;
        case COAP_RES_MODEL_COLUMN_FORMAT:
            return item.contextFormat;
        case COAP_RES_MODEL_COLUMN_PAYLOAD:
            return QVariant::fromValue(item.payload);
        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (column == COAP_RES_MODEL_COLUMN_PAYLOAD) {
            return int(Qt::AlignVCenter | Qt::AlignLeft);
        } else {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

QVariant CoAPResourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case COAP_RES_MODEL_COLUMN_DESCRIPTION:
                return tr("Description");
            case COAP_RES_MODEL_COLUMN_URI:
                return tr("URI Path");
            case COAP_RES_MODEL_COLUMN_FORMAT:
                return tr("Context Format");
            case COAP_RES_MODEL_COLUMN_PAYLOAD:
                return tr("Payload (Hex)");
            default:
                return QVariant();
            }
        }

        if (role == Qt::TextAlignmentRole) {
            if (section == COAP_RES_MODEL_COLUMN_PAYLOAD) {
                return int(Qt::AlignVCenter | Qt::AlignLeft);
            } else {
                return Qt::AlignCenter;
            }
        }
    } else {
        if (role == Qt::DisplayRole) {
            return QString::number(section + 1);
        }
    }

    return QVariant();
}

} // namespace xCoAP